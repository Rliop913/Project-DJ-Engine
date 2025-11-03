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

#define NOMINMAX
#include <Windows.h>

namespace PDJE_IPC {

namespace fs              = std::filesystem;
constexpr int PDJE_NO_IPC = 2;
constexpr int PDJE_IPC_R  = 1;
constexpr int PDJE_IPC_RW = 0;

template <typename T, int MEM_PROT_FLAG> class SharedMem {
  public:
    SharedMem()
    {
        startlog();
    }

    SharedMem(const SharedMem &) = delete;
    SharedMem &
    operator=(const SharedMem &) = delete;

    T *ptr = nullptr;

    HANDLE memory_handle = nullptr;

    uint64_t data_count = 0;

    bool
    GetIPCSharedMemory(const fs::path &memfd_name)
    {

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

            ptr = new T[count];

            data_count = count;
            return true;
        }

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
    }

    ~SharedMem()
    {
        if (MEM_PROT_FLAG == PDJE_NO_IPC) {
            if (ptr) {
                delete ptr;
            }
            return;
        }

        if (ptr) {
            UnmapViewOfFile(ptr);
        }
        if (memory_handle) {
            CloseHandle(memory_handle);
        }
    }
};

}; // namespace PDJE_IPC_UTILS