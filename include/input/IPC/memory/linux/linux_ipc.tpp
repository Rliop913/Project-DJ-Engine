#pragma once
#include "PDJE_INPUT_PROCESS_HASH.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <picosha2.h>

#include <string>
#include <vector>

#include <sys/mman.h>
#include <unistd.h>

// evade lsp flag. disable red lines temporary. do not activate on build step.
// #define EVADE_LSP

#ifdef EVADE_LSP
#include "ipc_shared_memory.hpp"
#endif

namespace PDJE_IPC {

// template <typename T, int MEM_PROT_FLAG> class SharedMem
template <typename T, int MEM_PROT_FLAG>
bool
SharedMem<T, MEM_PROT_FLAG>::GetIPCSharedMemory(const fs::path &memfd_name)
{

    return false; // todo -impl
}
template <typename T, int MEM_PROT_FLAG>
bool
SharedMem<T, MEM_PROT_FLAG>::MakeIPCSharedMemory(const fs::path &memfd_name,
                                                 const uint64_t  count)
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
        if (munmap(ptr, sizeof(T) * data_count) == -1) {
            critlog("failed to munmap");
        }
    }
    if (FD != -1) {
        if (close(FD) == -1) {
            critlog("failed to close FD");
        }
    }
}
}; // namespace PDJE_IPC