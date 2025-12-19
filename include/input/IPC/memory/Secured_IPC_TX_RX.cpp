#include "Secured_IPC_TX_RX.hpp"

namespace PDJE_CRYPTO {

void
TX_RX::AddFunction(const HEADER header, FEATURE feature)
{
    feature_map[header] = feature;
}

bool
TX_RX::Send(const HEADER header, const std::string &body)
{
    if (body.size() > BODY_SIZE) {
        critlog("failed to send buffer with TX_RX. body size exceeded maximum "
                "size.");
        return false;
    }
    PDJE_IPC::SCOPE_LOCK txlock(TXM);
    auto                 res   = aead.EncryptAndPack(body);
    ENCRYPT_RES_SIZE     bsize = res.size();
    if (bsize > ENCRYPT_MAX_SIZE) {
        critlog("failed to send buffer with TX_RX. encrypted body size "
                "exceeded maximum size.");

        return false;
    }

    std::array<std::byte, MSG_MAX_SIZE> packet{};
    std::memcpy(packet.data(), &header, sizeof(HEADER));
    std::memcpy(
        packet.data() + sizeof(HEADER), &bsize, sizeof(ENCRYPT_RES_SIZE));
    std::memcpy(packet.data() + sizeof(HEADER) + sizeof(ENCRYPT_RES_SIZE),
                res.data(),
                res.size());
    std::memcpy(TXBuf.ptr, packet.data(), MSG_MAX_SIZE);

    return true;
}

void
TX_RX::Listen()
{
    if (!listen_worker) {
        worker_switch = true;
        listen_worker = std::thread([this]() {
            HEADER           header;
            ENCRYPT_RES_SIZE bsize;
            std::string      body;
            body.reserve(ENCRYPT_MAX_SIZE);
            while (worker_switch) {
                {
                    PDJE_IPC::SCOPE_LOCK rxlock(RXM);
                    std::memcpy(&header, RXBuf.ptr, sizeof(HEADER));
                    if (header != 0) {
                        std::memcpy(&bsize,
                                    RXBuf.ptr + sizeof(HEADER),
                                    sizeof(ENCRYPT_RES_SIZE));
                        body.clear();
                        if (bsize > ENCRYPT_MAX_SIZE) {
                            std::memset(RXBuf.ptr, 0, MSG_MAX_SIZE);
                            continue;
                        }
                        body.resize(bsize);
                        std::memcpy(body.data(),
                                    RXBuf.ptr + sizeof(HEADER) +
                                        sizeof(ENCRYPT_RES_SIZE),
                                    body.size());

                        std::memset(RXBuf.ptr, 0, sizeof(HEADER));
                    }
                } // lock and get datas.

                if (header != 0) {
                    auto func = feature_map.find(header);
                    if (func != feature_map.end()) {
                        func->second(aead.UnpackAndDecrypt(body));
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }
}

TX_RX::~TX_RX()
{
    if (listen_worker) {
        if (listen_worker->joinable()) {
            worker_switch = false;
            listen_worker->join();
        }
    }
}
}; // namespace PDJE_CRYPTO