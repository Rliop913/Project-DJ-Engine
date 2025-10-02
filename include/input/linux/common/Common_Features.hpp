#pragma once
#include <arpa/inet.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <netinet/in.h>
#include <string>
#include <string_view>
#include <sys/socket.h>

#include <nlohmann/json.hpp>
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
        return -1;
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
        return -1;
    }
    data.resize(len);

    if (RecvByte(dest_fd, data.data(), len) < 0) {
        return errno;
    }
    return 0;
}

static std::string
ParseMsg(std::string_view msg)
{
}

}; // namespace Common_Features