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
#endif
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

    T       *ptr        = nullptr;
    uint64_t data_count = 0;
#ifdef WIN32
    HANDLE memory_handle = nullptr;
#elif defined(__linux__)
    int FD = -1;
#endif

    bool
    GetIPCSharedMemory(const fs::path &memfd_name);
    //     {
    // #ifdef WIN32
    // #include "windows_get_ipc_shmem.inl"
    // #elif defined(__linux__)
    // #include "linux_get_ipc_shmem.inl"
    // #endif
    //     }

    bool
    MakeIPCSharedMemory(const fs::path &memfd_name, const uint64_t count);
    //     {
    //         if (count == 0) {
    //             return false;
    //         }
    //         data_count = count;

    //         const uint64_t bsize = sizeof(T);
    //         if (count > ((std::numeric_limits<uint64_t>::max)() / bsize)) {
    //             return false;
    //         }

    //         if (MEM_PROT_FLAG == PDJE_NO_IPC) {

    //             ptr = new T[count];

    //             data_count = count;
    //             return true;
    //         }
    // #ifdef WIN32
    // #include "windows_make_ipc_shmem.inl"
    // #elif defined(__linux__)
    // #include "linux_make_ipc_shmem.inl"
    // #endif
    //     }

    ~SharedMem();
    //     {
    //         if (MEM_PROT_FLAG == PDJE_NO_IPC) {
    //             if (ptr) {
    //                 delete ptr;
    //             }
    //             return;
    //         }
    // #ifdef WIN32
    //         if (ptr) {
    //             UnmapViewOfFile(ptr);
    //         }
    //         if (memory_handle) {
    //             CloseHandle(memory_handle);
    //         }
    // #elif defined(__linux__)
    //         if (ptr) {
    //             if (munmap(ptr, sizeof(T) * data_count) == -1) {
    //                 critlog("failed to munmap");
    //             }
    //         }
    //         if (FD != -1) {
    //             if (close(FD) == -1) {
    //                 critlog("failed to close FD");
    //             }
    //         }
    // #endif
    //     }
};

}; // namespace PDJE_IPC
#ifdef WIN32
#include "windows_ipc.inl"
#elif defined(__linux__)
#include "linux_ipc.tpp"
#endif