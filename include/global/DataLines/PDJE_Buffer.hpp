#pragma once

#include "PDJE_EXPORT_SETTER.hpp"
#include "ipc_shared_memory.hpp"
#include <atomic>
#include <memory_resource>
#include <random>
#include <vector>

template <typename T> class PDJE_Buffer_Arena {
  private:
    // std::pmr::unsynchronized_pool_resource arena;
    // std::pmr::polymorphic_allocator<T>     allocator;
    // std::pmr::vector<T>                    buf1;
    // std::pmr::vector<T>                    buf2;
    PDJE_IPC::SharedMem<T, PDJE_IPC::PDJE_IPC_RW>        buf1;
    PDJE_IPC::SharedMem<T, PDJE_IPC::PDJE_IPC_RW>        buf2;
    PDJE_IPC::SharedMem<uint64_t, PDJE_IPC::PDJE_IPC_RW> first_count;
    PDJE_IPC::SharedMem<uint64_t, PDJE_IPC::PDJE_IPC_RW> second_count;

    PDJE_IPC::SharedMem<std::atomic_flag, PDJE_IPC::PDJE_IPC_RW> lock;
    PDJE_IPC::SharedMem<uint8_t, PDJE_IPC::PDJE_IPC_RW>          buf_first;
    // std::atomic_flag                       lock      = ATOMIC_FLAG_INIT;
    // bool                                   buf_first = true;

    T       *buffer_first_pointer_cache  = nullptr;
    T       *buffer_second_pointer_cache = nullptr;
    uint64_t BUFFER_COUNT                = 0;

  public:
    std::filesystem::path ID;
    void
    Write(const T &data);

    std::pair<T *, uint64_t>
    Get();

    PDJE_Buffer_Arena(const uint64_t count)
    {
        BUFFER_COUNT = count;
        std::random_device                 rd;
        std::mt19937                       gen(rd());
        std::uniform_int_distribution<int> dis(0, INT_MAX);
        ID = std::filesystem::path(std::to_string(dis(gen)));
        buf1.MakeIPCSharedMemory(
#ifdef WIN32
            std::filesystem::path(L"Local\\PDJE_INPUT_SHMEM_BUF_FIRST" +
                                  ID.wstring()),
#else
            std::filesystem::path("PDJE_INPUT_SHMEM_BUF_FIRST"),
#endif
            BUFFER_COUNT);
        buf2.MakeIPCSharedMemory(
#ifdef WIN32
            std::filesystem::path(L"Local\\PDJE_INPUT_SHMEM_BUF_SECOND" +
                                  ID.wstring()),
#else
            std::filesystem::path("PDJE_INPUT_SHMEM_BUF_SECOND" + ID.string()),
#endif
            BUFFER_COUNT);
        first_count.MakeIPCSharedMemory(
#ifdef WIN32
            std::filesystem::path(L"Local\\PDJE_INPUT_SHMEM_BUF_COUNT_FIRST" +
                                  ID.wstring()),
#else
            std::filesystem::path("PDJE_INPUT_SHMEM_BUF_COUNT_FIRST" +
                                  ID.string()),
#endif
            1);
        second_count.MakeIPCSharedMemory(
#ifdef WIN32
            std::filesystem::path(L"Local\\PDJE_INPUT_SHMEM_BUF_COUNT_SECOND" +
                                  ID.wstring()),
#else
            std::filesystem::path("PDJE_INPUT_SHMEM_BUF_COUNT_SECOND" +
                                  ID.string()),
#endif
            1);
        lock.MakeIPCSharedMemory(
#ifdef WIN32
            std::filesystem::path(L"Local\\PDJE_INPUT_SHMEM_LOCK" +
                                  ID.wstring()),
#else
            std::filesystem::path("PDJE_INPUT_SHMEM_LOCK" + ID.string()),
#endif
            1);
        buf_first.MakeIPCSharedMemory(
#ifdef WIN32
            std::filesystem::path(L"Local\\PDJE_INPUT_SHMEM_BUFFER_SWITCH" +
                                  ID.wstring()),
#else
            std::filesystem::path("PDJE_INPUT_SHMEM_BUFFER_SWITCH" +
                                  ID.string()),
#endif
            1);
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
        *buffer_first_pointer_cache = data;
        ++buffer_first_pointer_cache;
        ++(*first_count.ptr);
    } else {
        if ((*second_count.ptr) >= BUFFER_COUNT) {
            lock.ptr->clear(std::memory_order_release); // unlock
            return;
        }
        *buffer_second_pointer_cache = data;
        ++buffer_second_pointer_cache;
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
        buffer_second_pointer_cache = buf2.ptr;
        (*second_count.ptr)         = 0;
    } else {
        buffer_first_pointer_cache = buf1.ptr;
        (*first_count.ptr)         = 0;
    }

    (*buf_first.ptr) = (*buf_first.ptr == 1) ? 0 : 1;

    lock.ptr->clear(std::memory_order_release);
    if ((*buf_first.ptr)) {
        return std::pair(buf2.ptr, (*second_count.ptr));
    } else {
        return std::pair(buf1.ptr, (*first_count.ptr));
    }
}