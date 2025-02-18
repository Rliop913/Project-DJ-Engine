#include "LINUX_INPUT.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/mman.h>
#include <chrono>

void
futex_wait(int* addr)
{
    syscall(SYS_futex, addr, FUTEX_WAIT, 0, NULL, NULL, 0);
}

void
futex_wake(int* addr)
{
    syscall(SYS_futex, addr, FUTEX_WAKE, 1, NULL, NULL, 0);
}


int main(int argc, char* argv[])
{

    std::string SHM_FUTEX_NAME = argv[1];
    std::string SHM_EVDEV_MIRROR_NAME = argv[2];
    std::string SHM_DELAY = "/PDJE_DELAY_CHECK";
    int delay_shm_fd = shm_open(SHM_DELAY.c_str(), O_RDWR, 0666);
    int futex_shm_fd = shm_open(SHM_FUTEX_NAME.c_str(), O_RDWR, 0666);
    int evdev_shm = shm_open(SHM_EVDEV_MIRROR_NAME.c_str(), O_RDWR, 0666);
    
    int* futexVar = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, futex_shm_fd, 0);
    int* tempstopper = (int*)mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, evdev_shm, 0);



    std::chrono::_V2::system_clock::time_point* delay_checker =
        (std::chrono::_V2::system_clock::time_point*)mmap(
            NULL, sizeof(std::chrono::_V2::system_clock::time_point),
            PROT_READ | PROT_WRITE, MAP_SHARED, delay_shm_fd, 0
        );
    while(true){
        (*delay_checker) = std::chrono::high_resolution_clock::now();
        futex_wake(futexVar);
        if(*tempstopper != 0){
            break;
        }
        sleep(1);
        std::cout<< "WAKEUP CALL" << std::endl;
    }
    
    close(futex_shm_fd);
    close(evdev_shm);
    munmap(futexVar, sizeof(int));
    munmap(tempstopper, sizeof(int));

    munmap(delay_checker, sizeof(std::chrono::_V2::system_clock::time_point));
    shm_unlink(SHM_FUTEX_NAME.c_str());
    shm_unlink(SHM_EVDEV_MIRROR_NAME.c_str());
    shm_unlink(SHM_DELAY.c_str());
}