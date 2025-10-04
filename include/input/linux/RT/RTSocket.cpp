#include "RTSocket.hpp"
#include "Common_Features.hpp"
#include <exception>
#include <stdexcept>
#include <string>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>

bool
RTSocket::FixCPU(int core_number)
{
    int valid_res = CoreValid(core_number);
    if (valid_res < 0) {
        switch (valid_res) {
        case PDJE_RT_ERROR::FAILED_TO_SCHED_GETAFFINITY:
            ErrMsg += "failed to sched getaffinity\n";
            break;
        case PDJE_RT_ERROR::FAILED_TO_SET_CPU_NUMBER:
            ErrMsg += "failed to set cpu number\n";
            break;
        }
        return false;
    }

    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);

    CPU_SET(core_number, &cpu_set);

    if (sched_setaffinity(0, sizeof(cpu_set), &cpu_set) != 0) {
        ErrMsg += "failed to set affinity\n";
        return false;
    }

    if (numa_available() != -1) {
        int node = numa_node_of_cpu(core_number);
        node     = node >= 0 ? node : 0;

        int policy = MPOL_BIND;

        bitmask *node_mask = numa_allocate_nodemask();

        if (!node_mask) {
            ErrMsg += "failed to allocate node mask\n";
            return false;
        } else {
            numa_bitmask_clearall(node_mask);
            numa_bitmask_setbit(node_mask, node);

            if (set_mempolicy(policy, node_mask->maskp, node_mask->size)) {
                ErrMsg += "set mem policy failed\n";
                return false;
            }
            numa_free_nodemask(node_mask);
        }
        return true;
    } else {
        ErrMsg += "numa unavailable\n";
        return true;
    }
}

int
RTSocket::CoreValid(int core_number)
{
    cpu_set_t allowed;
    CPU_ZERO(&allowed);

    if (sched_getaffinity(0, sizeof(allowed), &allowed) != 0) {
        return PDJE_RT_ERROR::FAILED_TO_SCHED_GETAFFINITY;
    }

    if (!CPU_ISSET(core_number, &allowed)) {

        for (int i = 0; i < CPU_SETSIZE; ++i) {
            if (CPU_ISSET(i, &allowed)) {
                core_number = i;
                break;
            }
        }
        if (!CPU_ISSET(core_number, &allowed)) {
            return PDJE_RT_ERROR::FAILED_TO_SET_CPU_NUMBER;
        }
    }
    return 0;
}
#include <iostream>
int
RTSocket::SocketOpen(const std::string &socket_path)
{
    importants.host_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    if (connect(importants.host_socket,
                reinterpret_cast<sockaddr *>(&addr),
                sizeof(addr)) < 0) {
        std::cerr << errno << std::endl;
        return errno;
    }
    return 0;
}

int
RTSocket::Communication()
{
    std::string msg;
    int         resFlag = 0;
    while (true) {
        msg.clear();
        resFlag = Common_Features::LPRecv(importants.host_socket, msg);
        if (resFlag != 0) {
            return resFlag;
        }
        int parseFlag = ParseMsg(msg);
        if (parseFlag > 0) {
            return 0;
        }

        if (parseFlag != 0) {
            if (!errorHandler.contains(parseFlag)) {
                throw std::runtime_error("failed to handle error." +
                                         std::to_string(parseFlag));
            }
            errorHandler.at(parseFlag)();
        }
    }
}

int
RTSocket::SocketClose()
{
    return close(importants.host_socket);
}
void
RTSocket::MLock()
{
    mlockall(MCL_CURRENT | MCL_FUTURE);
}

RTSocket::~RTSocket()
{
    SocketClose();
    munlockall();
}

RTSocket::RTSocket(const std::string &socket_path)
{
    if (!FixCPU()) {
        throw std::runtime_error("failed to fix cpu. ERRMSG:" + ErrMsg);
    }
    MLock();
    SocketOpen(socket_path);
    RegisterFunctions();
}
#include <iostream> //debug
int
RTSocket::ParseMsg(const std::string &raw_json_msg)
{
    nj parsed;
    try {
        parsed = nj::parse(raw_json_msg);

    } catch (const std::exception &e) {
        return PDJE_RT_ERROR::FAILED_TO_PARSE_JSON;
    }
    if (parsed.contains("HEAD") && parsed.contains("BODY")) {
        if (parsed["HEAD"].is_string() && parsed["BODY"].is_array() &&
            functionRegistry.contains(parsed["HEAD"].get<std::string>())) {
            return functionRegistry[parsed["HEAD"].get<std::string>()](
                parsed["BODY"].get<std::vector<std::string>>());

        } else {
            std::cout << raw_json_msg << std::endl;

            return PDJE_RT_ERROR::FAILED_TO_PARSE_JSON_HEAD_BODY;
        }
    } else {
        return PDJE_RT_ERROR::INVALID_JSON_FORMAT;
    }
}