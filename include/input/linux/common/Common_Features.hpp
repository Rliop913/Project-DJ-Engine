#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <string>
#include <sys/socket.h>
#include <vector>
using nj = nlohmann::json;
enum PDJE_RT_ERROR {
    FAILED_TO_PARSE_JSON                     = -1,
    FAILED_TO_PARSE_JSON_HEAD_BODY           = -2,
    INVALID_JSON_FORMAT                      = -3,
    FAILED_TO_SCHED_GETAFFINITY              = -4,
    FAILED_TO_SET_CPU_NUMBER                 = -5,
    FAILED_TO_SEND_RECV__DATA_LENGTH_IS_ZERO = -6,
    FAILED_TO_SET_DEV__INVALID_DATA          = -7
};

namespace Common_Features {

static int
SendByte(int dest_fd, const void *data, size_t len)
{
    const char *p = reinterpret_cast<const char *>(data);
    while (len > 0) {
        auto sended_bytes = send(dest_fd, p, len, MSG_NOSIGNAL);

        if (sended_bytes < 0) {
            if (errno == EINTR)
                continue; // ignore signal
            if (errno == EAGAIN)
                continue; // nonblock
            return -errno;
        } else if (sended_bytes == 0) {
            return -EPIPE;
        }
        p += static_cast<size_t>(sended_bytes);
        len -= static_cast<size_t>(sended_bytes);
    }
    return len;
}

static int
RecvByte(int dest_fd, void *data, size_t len)
{
    char *p = reinterpret_cast<char *>(data);
    while (len > 0) {
        auto readed_bytes = recv(dest_fd, p, len, 0);

        if (readed_bytes < 0) {
            if (errno == EINTR)
                continue; // ignore signal
            if (errno == EAGAIN)
                continue; // nonblock
            return -errno;
        } else if (readed_bytes == 0) {
            return -EPIPE;
        }
        p += static_cast<size_t>(readed_bytes);
        len -= static_cast<size_t>(readed_bytes);
    }
    return len;
}

/// length prefixed message send
static int
LPSend(int dest_fd, const std::string &data)
{
    auto len = data.size();
    if (len > std::numeric_limits<uint32_t>::max()) {
        return -EMSGSIZE;
    }
    if (len == 0) {
        return PDJE_RT_ERROR::FAILED_TO_SEND_RECV__DATA_LENGTH_IS_ZERO;
    }
    auto net_len = htonl(static_cast<uint32_t>(len));
    if (SendByte(dest_fd, &net_len, sizeof(uint32_t)) < 0) {
        return errno;
    }
    if (SendByte(dest_fd, data.data(), len) < 0) {
        return errno;
    }
    return 0;
}

static int
LPRecv(int dest_fd, std::string &data)
{
    uint32_t net_len;
    if (RecvByte(dest_fd, &net_len, sizeof(uint32_t)) < 0) {
        return errno;
    }
    auto len = ntohl(net_len);
    if (len == 0) {
        return PDJE_RT_ERROR::FAILED_TO_SEND_RECV__DATA_LENGTH_IS_ZERO;
    }
    data.resize(len);

    if (RecvByte(dest_fd, data.data(), len) < 0) {
        return errno;
    }
    return 0;
}

static std::string
MakeMSG(const std::string &head, const std::vector<std::string> &datas)
{
    nj j_obj;
    j_obj["HEAD"] = head;
    j_obj["BODY"] = datas;
    return j_obj.dump();
}

static std::string
MakeMSG(const std::string &head, const std::string &msg)
{
    std::vector<std::string> tempVec;
    tempVec.push_back(msg);
    nj j_obj;
    j_obj["HEAD"] = head;
    j_obj["BODY"] = tempVec;
    return j_obj.dump();
}

static std::vector<std::string>
ReadMSG(const std::string &head, const std::string &raw_json_msg)
{
    nj parsed;
    try {
        parsed = nj::parse(raw_json_msg);

    } catch (const std::exception &e) {
        return std::vector<std::string>();
    }
    if (parsed.contains("HEAD") && parsed.contains("BODY")) {
        if (parsed["HEAD"].is_string() && parsed["BODY"].is_array()) {
            if (parsed["HEAD"].get<std::string>() == head) {
                return parsed["BODY"].get<std::vector<std::string>>();
            } else {
                return std::vector<std::string>();
            }

        } else {

            return std::vector<std::string>();
        }
    } else {
        return std::vector<std::string>();
    }
}

// void
// TOCTOU_DODGE()
// {
//     // todo- impl
//     // impl with hash compare - random re-compare + random fake execution +
//     // random sleep time
// }

}; // namespace Common_Features