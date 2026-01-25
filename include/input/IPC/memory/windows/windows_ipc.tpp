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


#include <string>
#include <vector>

// evade lsp flag. disable red lines temporary. do not activate on build step.
// #define EVADE_LSP

#ifdef EVADE_LSP
#include "ipc_shared_memory.hpp"
#endif

#define NOMINMAX
#include <Windows.h>

namespace PDJE_IPC {

namespace fs              = std::filesystem;


template <typename T, int MEM_PROT_FLAG>

    bool
    SharedMem<T, MEM_PROT_FLAG>::GetIPCSharedMemory(const fs::path &memfd_name, const uint64_t count)
    {

        constexpr DWORD mapAccess =
            (MEM_PROT_FLAG == 1) ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS;
        data_count = count;
        memory_handle =
            OpenFileMappingW(mapAccess, FALSE, memfd_name.wstring().c_str());
        if (!memory_handle) {
            critlog("failed to get open file mapping");
            return false;
        }
        ptr = static_cast<T*>( MapViewOfFile(memory_handle, mapAccess, 0, 0, 0));
        if (!ptr) {
            CloseHandle(memory_handle);
            critlog("failed to map view of file.");
            return false;
        }
        return true;
    }
template <typename T, int MEM_PROT_FLAG>
    bool
    SharedMem<T, MEM_PROT_FLAG>::MakeIPCSharedMemory(const fs::path &memfd_name, const uint64_t count)
    {
        if (count == 0) {
            return false;
        }
        data_count = count;

        const uint64_t bsize = sizeof(T);
        if (count > ((std::numeric_limits<uint64_t>::max)() / bsize)) {
            critlog("exceeded numeric limits for ipc shared memory. size, count: ");
            critlog(bsize);
            critlog(count);
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
            critlog("failed to create file mapping.");
            return false;
        }

        ptr = reinterpret_cast<T *>(
            MapViewOfFile(memory_handle, mapAccess, 0, 0, data_max_length));

        if (!ptr) {
            CloseHandle(memory_handle);
            memory_handle = nullptr;
            critlog("failed to map view of file.");
            return false;
        }

        return true;
    }
template <typename T, int MEM_PROT_FLAG>
    SharedMem<T, MEM_PROT_FLAG>::~SharedMem()
    {
        if (MEM_PROT_FLAG == PDJE_NO_IPC) {
            if (ptr) {
                delete[] ptr;
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


}; // namespace PDJE_IPC_UTILS