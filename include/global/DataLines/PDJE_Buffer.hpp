#pragma once

#include "PDJE_EXPORT_SETTER.hpp"
#include "ipc_shared_memory.hpp"
#include <atomic>
#include <memory_resource>
#include <random>
#include <vector>
namespace fs = std::filesystem;
template <typename T> class PDJE_Buffer_Arena {
  private:
    PDJE_IPC::SharedMem<T, PDJE_IPC::PDJE_IPC_RW>        buf1;
    PDJE_IPC::SharedMem<T, PDJE_IPC::PDJE_IPC_RW>        buf2;
    PDJE_IPC::SharedMem<uint64_t, PDJE_IPC::PDJE_IPC_RW> first_count;
    PDJE_IPC::SharedMem<uint64_t, PDJE_IPC::PDJE_IPC_RW> second_count;

    PDJE_IPC::SharedMem<std::atomic_flag, PDJE_IPC::PDJE_IPC_RW> lock;
    PDJE_IPC::SharedMem<uint8_t, PDJE_IPC::PDJE_IPC_RW>          buf_first;

    T       *buffer_first_pointer_cache  = nullptr;
    T       *buffer_second_pointer_cache = nullptr;    
    
    public:
    uint64_t BUFFER_COUNT                = 0;
    std::filesystem::path ID;
    void
    Write(const T &data);

    std::pair<T *, uint64_t>
    Get();

    PDJE_Buffer_Arena(const std::string& id, const uint64_t count)
    {
        BUFFER_COUNT = count;
        buf1.GetIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_FIRST") + id), BUFFER_COUNT);
        buf2.GetIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_SECOND") + id), BUFFER_COUNT);
        first_count.GetIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_COUNT_FIRST") + id), 1);
        second_count.GetIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_COUNT_SECOND") + id), 1);
        lock.GetIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_LOCK") + id), 1);
        buf_first.GetIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUFFER_SWITCH") + id), 1);
        buffer_first_pointer_cache  = buf1.ptr;
        buffer_second_pointer_cache = buf2.ptr;
    }

    PDJE_Buffer_Arena(const uint64_t count)
    {
        BUFFER_COUNT = count;
        std::random_device                 rd;
        std::mt19937                       gen(rd());
        std::uniform_int_distribution<int> dis(0, INT_MAX);
        ID = fs::path(std::to_string(dis(gen)));
        buf1.MakeIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_FIRST") +
                                  ID.string()),BUFFER_COUNT);
        buf2.MakeIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_SECOND") +
                                  ID.string()), BUFFER_COUNT);
        first_count.MakeIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_COUNT_FIRST") +
                                  ID.string()), 1);
        second_count.MakeIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUF_COUNT_SECOND") +
                                  ID.string()), 1);
        lock.MakeIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_LOCK") +
                                  ID.string()), 1);
        buf_first.MakeIPCSharedMemory(
            fs::path(std::string("PDJE_INPUT_SHMEM_BUFFER_SWITCH") +
                                  ID.string()), 1);
        buffer_first_pointer_cache  = buf1.ptr;
        buffer_second_pointer_cache = buf2.ptr;
        new (lock.ptr) std::atomic_flag();
        *first_count.ptr  = 0;
        *second_count.ptr = 0;
        *buf_first.ptr    = 1;

        lock.ptr->clear(std::memory_order_relaxed);
    }

    ~PDJE_Buffer_Arena() = default;
};
template <typename T>
void
PDJE_Buffer_Arena<T>::Write(const T &data)
{

    while (lock.ptr->test_and_set(std::memory_order_acquire)) {
    }
    // locked
    if (*buf_first.ptr == 1) {
        if ((*first_count.ptr) >= BUFFER_COUNT) {
            lock.ptr->clear(std::memory_order_release); // unlock
            return;
        }
        
        buf1.ptr[*(first_count.ptr)] = data;
        ++(*first_count.ptr);
        

    } else {
        if ((*second_count.ptr) >= BUFFER_COUNT) {
            lock.ptr->clear(std::memory_order_release); // unlock
            return;
        }
        buf2.ptr[(*second_count.ptr)] = data;
        ++(*second_count.ptr);
    }
    lock.ptr->clear(std::memory_order_release); // unlock
}

template <typename T>
std::pair<T *, uint64_t>
PDJE_Buffer_Arena<T>::Get()
{
    while (lock.ptr->test_and_set(std::memory_order_acquire)) {
    }
    if (*buf_first.ptr == 1) {
        (*second_count.ptr)         = 0;
    } else {
        (*first_count.ptr)         = 0;
    }

    (*buf_first.ptr) = (*buf_first.ptr == 1) ? 0 : 1;

    lock.ptr->clear(std::memory_order_release);
    if ((*buf_first.ptr) == 1) {
        return std::pair(buffer_second_pointer_cache, (*second_count.ptr));
    } else {
        return std::pair(buffer_first_pointer_cache, (*first_count.ptr));
    }
}