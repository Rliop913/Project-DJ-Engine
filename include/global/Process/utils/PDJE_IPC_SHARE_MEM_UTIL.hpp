#pragma once

#include "PDJE_INPUT_PROCESS_HASH.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <picosha2.h>

#include <string>
#include <vector>

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>

#else
#include <sys/mman.h>
#include <unistd.h>
#endif
namespace PDJE_IPC_UTILS {

namespace fs              = std::filesystem;
constexpr int PDJE_NO_IPC = 2;
constexpr int PDJE_IPC_R  = 1;
constexpr int PDJE_IPC_RW = 0;

template <typename T, int MEM_PROT_FLAG> class IPCSharedMem {
  public:
    IPCSharedMem()
    {
        startlog();
    }

    IPCSharedMem(const IPCSharedMem &) = delete;
    IPCSharedMem &
    operator=(const IPCSharedMem &) = delete;

    T *ptr = nullptr;

#ifdef WIN32
    HANDLE memory_handle = nullptr;
#else
    int FD = -1;
#endif
    uint64_t data_count = 0;

    bool
    GetIPCSharedMemory(const fs::path &memfd_name)
    {

#ifdef WIN32
        constexpr DWORD mapAccess =
            (MEM_PROT_FLAG == 1) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

        memory_handle =
            OpenFileMappingW(mapAccess, FALSE, memfd_name.wstring().c_str());
        if (!memory_handle) {
            return false;
        }
        ptr = MapViewOfFile(memory_handle, mapAccess, 0, 0, 0);
        if (!ptr) {
            CloseHandle(memory_handle);
            return false;
        }
        return true;
#else
        return false; // todo -impl
#endif
    }

    bool
    MakeIPCSharedMemory(const fs::path &memfd_name, const uint64_t count)
    {
        if (count == 0) {
            return false;
        }
        data_count = count;

        const uint64_t bsize = sizeof(T);
        if (count > ((std::numeric_limits<uint64_t>::max)() / bsize)) {
            return false;
        }

        if (MEM_PROT_FLAG == PDJE_NO_IPC) {
            
            ptr        = new T[count];
            
            data_count = count;
            return true;
        }

#ifdef WIN32

        SECURITY_ATTRIBUTES sa{};
        sa.nLength              = sizeof(sa);
        sa.bInheritHandle       = FALSE;
        sa.lpSecurityDescriptor = nullptr;

        constexpr DWORD flProtect =
            (MEM_PROT_FLAG == 1) ? PAGE_READONLY : PAGE_READWRITE;
        constexpr DWORD mapAccess =
            (MEM_PROT_FLAG == 1) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;

        const uint64_t data_max_length = bsize * count;
        memory_handle =
            CreateFileMappingW(INVALID_HANDLE_VALUE,
                               &sa,
                               flProtect,
                               (DWORD)(data_max_length >> 32),
                               (DWORD)(data_max_length & 0xFFFFFFFF),
                               memfd_name.wstring().c_str());
        if (!memory_handle) {
            return false;
        }

        ptr = reinterpret_cast<T *>(
            MapViewOfFile(memory_handle, mapAccess, 0, 0, data_max_length));

        if (!ptr) {
            CloseHandle(memory_handle);
            memory_handle = nullptr;
            return false;
        }

        return true;

#else
        FD = memfd_create(memfd_name.string().c_str(), MFD_CLOEXEC);
        if (FD < 0) {
            critlog("memfd not created");
            return false;
        }

        uint64_t byte_length = bsize * count;
        if (ftruncate(FD, static_cast<off_t>(byte_length)) != 0) {
            close(FD);
            FD = -1;
            critlog("ftruncate failed");
            return false;
        }
        int   prot = (MEM_PROT_FLAG == 1) ? PROT_READ : PROT_READ | PROT_WRITE;
        void *p    = mmap(nullptr, byte_length, prot, MAP_SHARED, FD, 0);
        if (p == MAP_FAILED) {
            close(FD);
            FD = -1;
            critlog("map failed");
            return false;
        }

        ptr = reinterpret_cast<T *>(p);

        return true;
#endif
    }

    ~IPCSharedMem()
    {
        if (MEM_PROT_FLAG == PDJE_NO_IPC) {
            if (ptr) {
                delete ptr;
            }
            return;
        }
#ifdef WIN32
        if (ptr) {
            UnmapViewOfFile(ptr);
        }
        if (memory_handle) {
            CloseHandle(memory_handle);
        }

#else

        if (ptr) {
            if (munmap(ptr, sizeof(T) * data_count) == -1) {
                critlog("failed to munmap");
            }
        }
        if (FD != -1) {
            if (close(FD) == -1) {
                critlog("failed to close FD");
            }
        }
#endif
    }
};

}; // namespace PDJE_IPC_UTILS