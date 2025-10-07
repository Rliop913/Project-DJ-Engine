#pragma once

#include <memory_resource>
#include <optional>
#include <atomic>

#define RESET_PMR_VECTOR(ARENA, VEC) std::pmr::vector<T>{&ARENA}.swap(VEC)  

template<typename T>
class PDJE_Buffer_Arena{
private:
    std::pmr::unsynchronized_pool_resource arena;
    std::pmr::vector<T> buf1;
    std::pmr::vector<T> buf2;
    std::atomic_flag lock = ATOMIC_FLAG_INIT;
    bool buf_first = true;
public:
    
    void Write(const T& data);
    std::pmr::vector<T>* Get();

    PDJE_Buffer_Arena():
    arena{},
    buf1(&arena),
    buf2(&arena)
    {
        buf1.reserve(2048);
        RESET_PMR_VECTOR(arena, buf1);
    }
    ~PDJE_Buffer_Arena() = default;
};

template<typename T>
void
PDJE_Buffer_Arena<T>::Write(const T& data)
{
    while(lock.test_and_set(std::memory_order_acquire)){}
    //locked
    if(buf_first){
        buf1.push_back(data);
    }
    else{
        buf2.push_back(data);
    }
    
    lock.clear(std::memory_order_release);
    //unlock
}

template<typename T>
std::pmr::vector<T>*
PDJE_Buffer_Arena<T>::Get()
{
    while(lock.test_and_set(std::memory_order_acquire)){}
    buf_first ? buf2.clear() : buf1.clear();
    buf_first = !buf_first;
    lock.clear(std::memory_order_release);
    return buf_first ? &buf2 : &buf1;
}