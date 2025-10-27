#pragma once

#include "PDJE_LOG_SETTER.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <picosha2.h>
#include <sstream>
#include <string>

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif
namespace PDJE_IPC_UTILS {

#ifdef WIN32
constexpr int PDJE_IPC_SHARE_MEM_RW = PROT_READ | PROT_WRITE;
constexpr int PDJE_IPC_SHARE_MEM_R  = PROT_READ;
constexpr int PDJE_IPC_SHARE_MEM_W  = PROT_WRITE;
#else
constexpr int PDJE_IPC_SHARE_MEM_RW = PROT_READ | PROT_WRITE;
constexpr int PDJE_IPC_SHARE_MEM_R  = PROT_READ;
constexpr int PDJE_IPC_SHARE_MEM_W  = PROT_WRITE;
#endif

namespace fs = std::filesystem;

template <typename T, int MEM_PROT_FLAG> class IPCSharedMem {
  public:
    IPCSharedMem()
    {
        startlog();
    }

    IPCSharedMem(const IPCSharedMem &) = delete;
    IPCSharedMem &
    operator=(const IPCSharedMem &) = delete;

    T     *ptr   = nullptr;
    int    FD    = -1;
    size_t count = 0;
    size_t bytes = 0;

    bool
    MakeIPCSharedMemory(const std::string &memfd_name, size_t data_length)
    {

        static_assert(
            std::is_trivially_copyable_v<T>,
            "Shared mem should contain trivially copyable types only.");
        if (data_length == 0) {
            critlog("data_length is zero");
            return false;
        }
        constexpr size_t Tsize    = sizeof(T);
        constexpr auto Max_length = std::numeric_limits<size_t>::max() / Tsize;
        if (Tsize != 0 && data_length > Max_length) {
            critlog("byte size exceeded size_t maximum value");
            return false;
        }

        FD = memfd_create(memfd_name.c_str(), MFD_CLOEXEC);
        if (FD < 0) {
            critlog("memfd not created");
            return false;
        }

        size_t byte_length = Tsize * data_length;
        if (ftruncate(FD, static_cast<off_t>(byte_length)) != 0) {
            close(FD);
            FD = -1;
            critlog("ftruncate failed");
            return false;
        }

        void *p = mmap(nullptr, byte_length, MEM_PROT_FLAG, MAP_SHARED, FD, 0);
        if (p == MAP_FAILED) {
            close(FD);
            FD = -1;
            critlog("map failed");
            return false;
        }

        ptr   = reinterpret_cast<T *>(p);
        count = data_length;
        bytes = byte_length;
        return true;
    }

    ~IPCSharedMem()
    {
        if (ptr && bytes > 0) {
            if (munmap(ptr, bytes) == -1) {
                critlog("failed to munmap");
            }
        }
        if (FD != -1) {
            if (close(FD) == -1) {
                critlog("failed to close FD");
            }
        }
    }
};

static inline fs::path
GetValidProcessExecutor()
{
    fs::path              current = fs::current_path();
    std::vector<fs::path> executables;
    for (auto &fp : fs::recursive_directory_iterator(
             current, fs::directory_options::skip_permission_denied)) {
        if (fs::is_regular_file(fp) &&
            fp.path().filename().string().find("PDJE_MODULE_INPUT_PROCESS") !=
                std::string::npos) {
            executables.push_back(fp.path());
        }
    }
    for (const auto &p : executables) {
        std::ifstream exfile(p, std::ios::binary | std::ios::ate);
        if (!exfile) {
            continue;
        }
        std::streamsize sz = exfile.tellg();
        exfile.seekg(0, std::ios::beg);
        std::vector<char> bin(sz);
        if (!exfile.read(bin.data(), sz)) {
            continue;
        }
        unsigned char hash[EVP_MAX_MD_SIZE];
        size_t        hashlen = sizeof(hash);

        EVP_Q_digest(NULL, "SHA256", NULL, bin.data(), sz, hash, &hashlen);
        hash;
    }
}

}; // namespace PDJE_IPC_UTILS