#pragma once
#include <cerrno>
#include <cstdint>

#include <stdexcept>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>

#include <systemd/sd-bus.h>

namespace rtkit {

static inline uint64_t
get_tid()
{
    const long tid = syscall(SYS_gettid);
    if (tid < 0) {
        throw std::runtime_error("SYS_gettid failed: " + std::to_string(errno));
    }
    return static_cast<uint64_t>(tid);
}

static inline void
throw_bus_err(const char *what, int r, sd_bus_error *err)
{
    std::string msg = what;
    msg += " (";
    msg += std::to_string(r);
    msg += ")";
    if (err && sd_bus_error_is_set(err)) {
        msg += ": ";
        msg += err->message ? err->message : "(no message)";
    }
    if (err) {
        sd_bus_error_free(err);
    }
    throw std::runtime_error(msg);
}

static inline int32_t
get_int32_property(sd_bus *bus, const char *property_name)
{
    sd_bus_error    err   = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = nullptr;

    int r = sd_bus_call_method(bus,
                               "org.freedesktop.RealtimeKit1",
                               "/org/freedesktop/RealtimeKit1",
                               "org.freedesktop.DBus.Properties",
                               "Get",
                               &err,
                               &reply,
                               "ss",
                               "org.freedesktop.RealtimeKit1",
                               property_name);
    if (r < 0) {
        if (reply) {
            sd_bus_message_unref(reply);
        }
        throw_bus_err("Properties.Get failed", r, &err);
    }

    int32_t value = 0;
    r = sd_bus_message_enter_container(reply, SD_BUS_TYPE_VARIANT, "i");
    if (r < 0) {
        sd_bus_error_free(&err);
        sd_bus_message_unref(reply);
        throw std::runtime_error(std::string("Failed to enter variant for ") +
                                 property_name);
    }

    r = sd_bus_message_read_basic(reply, SD_BUS_TYPE_INT32, &value);
    if (r < 0) {
        sd_bus_error_free(&err);
        sd_bus_message_unref(reply);
        throw std::runtime_error(
            std::string("Failed to read property value for ") + property_name);
    }

    r = sd_bus_message_exit_container(reply);
    if (r < 0) {
        sd_bus_error_free(&err);
        sd_bus_message_unref(reply);
        throw std::runtime_error(std::string("Failed to exit variant for ") +
                                 property_name);
    }

    sd_bus_error_free(&err);
    sd_bus_message_unref(reply);
    return value;
}

static inline uint32_t
get_max_realtime_priority_legacy_method(sd_bus *bus)
{
    sd_bus_error    err   = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = nullptr;

    int r = sd_bus_call_method(bus,
                               "org.freedesktop.RealtimeKit1",
                               "/org/freedesktop/RealtimeKit1",
                               "org.freedesktop.RealtimeKit1",
                               "GetMaxRealtimePriority",
                               &err,
                               &reply,
                               "");
    if (r < 0) {
        if (reply) {
            sd_bus_message_unref(reply);
        }
        throw_bus_err("GetMaxRealtimePriority failed", r, &err);
    }

    uint32_t prio = 0;
    r             = sd_bus_message_read(reply, "u", &prio);
    sd_bus_message_unref(reply);
    if (r < 0)
        throw std::runtime_error(
            "Failed to parse GetMaxRealtimePriority reply");
    return prio;
}

static inline int32_t
get_min_nice_level_legacy_method(sd_bus *bus)
{
    sd_bus_error    err   = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = nullptr;

    int r = sd_bus_call_method(bus,
                               "org.freedesktop.RealtimeKit1",
                               "/org/freedesktop/RealtimeKit1",
                               "org.freedesktop.RealtimeKit1",
                               "GetMinNiceLevel",
                               &err,
                               &reply,
                               "");
    if (r < 0) {
        if (reply) {
            sd_bus_message_unref(reply);
        }
        throw_bus_err("GetMinNiceLevel failed", r, &err);
    }

    int32_t nicev = 0;
    r             = sd_bus_message_read(reply, "i", &nicev);
    sd_bus_message_unref(reply);
    if (r < 0)
        throw std::runtime_error("Failed to parse GetMinNiceLevel reply");
    return nicev;
}

// Query helpers (optional)
static inline uint32_t
get_max_realtime_priority(sd_bus *bus)
{
    try {
        const int32_t property_prio =
            get_int32_property(bus, "MaxRealtimePriority");
        if (property_prio <= 0) {
            throw std::runtime_error(
                "MaxRealtimePriority property returned non-positive value");
        }
        return static_cast<uint32_t>(property_prio);
    } catch (const std::exception &prop_err) {
        try {
            const uint32_t method_prio =
                get_max_realtime_priority_legacy_method(bus);
            if (method_prio == 0) {
                throw std::runtime_error(
                    "GetMaxRealtimePriority returned zero value");
            }
            return method_prio;
        } catch (const std::exception &legacy_err) {
            throw std::runtime_error(
                std::string("Failed to query MaxRealtimePriority. property: ") +
                prop_err.what() + "; legacy method: " + legacy_err.what());
        }
    }
}

static inline int32_t
get_min_nice_level(sd_bus *bus)
{
    try {
        return get_int32_property(bus, "MinNiceLevel");
    } catch (const std::exception &prop_err) {
        try {
            return get_min_nice_level_legacy_method(bus);
        } catch (const std::exception &legacy_err) {
            throw std::runtime_error(
                std::string("Failed to query MinNiceLevel. property: ") +
                prop_err.what() + "; legacy method: " + legacy_err.what());
        }
    }
}

// Make current thread SCHED_FIFO realtime with given priority (1..99, but rtkit
// caps it)

static inline void
make_current_thread_realtime(uint32_t priority)
{
    sd_bus         *bus   = nullptr;
    sd_bus_error    err   = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = nullptr;

    int r = sd_bus_open_system(&bus);
    if (r < 0)
        throw std::runtime_error("sd_bus_open_system failed: " +
                                 std::to_string(r));
    if (priority == 0) {
        priority = 1;
    }

    try {
        const uint64_t tid = get_tid();

        // optional: clamp to rtkit max
        try {
            const uint32_t max_prio = get_max_realtime_priority(bus);
            if (priority > max_prio)
                priority = max_prio;
        } catch (const std::exception &) {
        }

        r = sd_bus_call_method(
            bus,
            "org.freedesktop.RealtimeKit1",
            "/org/freedesktop/RealtimeKit1",
            "org.freedesktop.RealtimeKit1",
            "MakeThreadRealtime",
            &err,
            &reply,
            "tu", // t = uint64 (thread id), u = uint32 (priority)
            tid,
            priority);

        if (r < 0) {
            throw_bus_err("MakeThreadRealtime failed", r, &err);
        }
    } catch (...) {
        if (reply) {
            sd_bus_message_unref(reply);
        }
        if (bus) {
            sd_bus_unref(bus);
        }
        throw;
    }

    if (reply) {
        sd_bus_message_unref(reply);
    }
    if (bus) {
        sd_bus_unref(bus);
    }
}

// Make current thread "high priority" by lowering nice (negative is higher
// priority). rtkit enforces a minimum nice level; values below that will be
// denied.
static inline void
make_current_thread_high_priority(int32_t nice_level)
{
    sd_bus         *bus   = nullptr;
    sd_bus_error    err   = SD_BUS_ERROR_NULL;
    sd_bus_message *reply = nullptr;

    int r = sd_bus_open_system(&bus);
    if (r < 0)
        throw std::runtime_error("sd_bus_open_system failed: " +
                                 std::to_string(r));

    try {
        const uint64_t tid = get_tid();

        // optional: clamp to rtkit min nice
        try {
            const int32_t min_nice = get_min_nice_level(bus);
            if (nice_level < min_nice)
                nice_level = min_nice;
        } catch (const std::exception &) {
            // query failure is non-fatal; try with provided level
        }

        r = sd_bus_call_method(
            bus,
            "org.freedesktop.RealtimeKit1",
            "/org/freedesktop/RealtimeKit1",
            "org.freedesktop.RealtimeKit1",
            "MakeThreadHighPriority",
            &err,
            &reply,
            "ti", // t = uint64 (thread id), i = int32 (nice level)
            tid,
            nice_level);

        if (r < 0) {
            throw_bus_err("MakeThreadHighPriority failed", r, &err);
        }
    } catch (...) {
        if (reply) {
            sd_bus_message_unref(reply);
        }
        if (bus) {
            sd_bus_unref(bus);
        }
        throw;
    }

    if (reply) {
        sd_bus_message_unref(reply);
    }
    if (bus) {
        sd_bus_unref(bus);
    }
}

} // namespace rtkit
