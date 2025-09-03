
.. _program_listing_file_include_tests_INPUT_TESTS_linux_I_WAITER.cpp:

Program Listing for File linux_I_WAITER.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_INPUT_TESTS_linux_I_WAITER.cpp>` (``include/tests/INPUT_TESTS/linux_I_WAITER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "LINUX_INPUT.hpp"
   #include <chrono>
   #include <fcntl.h>
   #include <iostream>
   #include <linux/futex.h>
   #include <spawn.h>
   #include <sys/mman.h>
   #include <sys/syscall.h>
   #include <sys/wait.h>
   #include <unistd.h>
   
   void
   futex_wait(int *addr)
   {
       syscall(SYS_futex, addr, FUTEX_WAIT, 0, NULL, NULL, 0);
   }
   
   void
   futex_wake(int *addr)
   {
       syscall(SYS_futex, addr, FUTEX_WAKE, 1, NULL, NULL, 0);
   }
   
   extern char **environ;
   
   int
   main()
   {
       std::string SHM_FUTEX_NAME        = "/PDJE_SHARED_MEMORY_FOR_FUTEX";
       std::string SHM_EVDEV_MIRROR_NAME = "/PDJE_SHARED_MEMORY_FOR_EVDEV_MIRROR";
       std::string SHM_DELAY             = "/PDJE_DELAY_CHECK";
       int delay_shm_fd = shm_open(SHM_DELAY.c_str(), O_CREAT | O_RDWR, 0666);
       int futex_shm_fd = shm_open(SHM_FUTEX_NAME.c_str(), O_CREAT | O_RDWR, 0666);
       int evdev_shm =
           shm_open(SHM_EVDEV_MIRROR_NAME.c_str(), O_CREAT | O_RDWR, 0666);
   
       auto temp = std::chrono::high_resolution_clock::now();
   
       ftruncate(futex_shm_fd, sizeof(int));
       ftruncate(evdev_shm, sizeof(int));
       ftruncate(delay_shm_fd, sizeof(std::chrono::_V2::system_clock::time_point));
   
       int *futexVar = (int *)mmap(
           NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, futex_shm_fd, 0);
       int *tempstopper = (int *)mmap(
           NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, evdev_shm, 0);
       int *evdevEventFD = (int *)mmap(
           NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, evdev_shm, 0);
   
       std::chrono::_V2::system_clock::time_point *delay_checker =
           (std::chrono::_V2::system_clock::time_point *)mmap(
               NULL,
               sizeof(std::chrono::_V2::system_clock::time_point),
               PROT_READ | PROT_WRITE,
               MAP_SHARED,
               delay_shm_fd,
               0);
       *tempstopper = 0;
       int times    = 10;
   
       pid_t pid;
   
       char *waker_args[] = { (char *)"pkexec",
                              (char *)"./testLinuxInput_waker",
                              (char *)SHM_FUTEX_NAME.c_str(),
                              (char *)SHM_EVDEV_MIRROR_NAME.c_str(),
                              nullptr };
   
       int status =
           posix_spawnp(&pid, "pkexec", nullptr, nullptr, waker_args, environ);
   
       // std::system(EXEC_COMMAND.c_str());
       std::chrono::_V2::system_clock::time_point delay;
       // std::chrono::_V2::system_clock::duration delayResult;
       std::chrono::duration<double, std::nano>  nano_duration;
       std::chrono::duration<double, std::micro> micro_duration;
       while (true) {
           futex_wait(futexVar);
           delay = std::chrono::high_resolution_clock::now();
           // delayResult =
           // std::chrono::duration_cast<std::chrono::nanoseconds>(delay -
           // (*delay_checker));
           nano_duration  = delay - (*delay_checker);
           micro_duration = delay - (*delay_checker);
           std::cout << "From Waiter, Delay(Nanosecond): " << nano_duration.count()
                     << std::endl;
           std::cout << "From Waiter, Delay(Microsecond): "
                     << micro_duration.count() << std::endl;
   
           if (times < 0) {
               *tempstopper = 1;
               sleep(2);
               break;
           }
           --times;
       }
       waitpid(pid, &status, 0);
       close(futex_shm_fd);
       close(evdev_shm);
       close(delay_shm_fd);
       munmap(futexVar, sizeof(int));
       munmap(tempstopper, sizeof(int));
       munmap(delay_checker, sizeof(std::chrono::_V2::system_clock::time_point));
       shm_unlink(SHM_FUTEX_NAME.c_str());
       shm_unlink(SHM_EVDEV_MIRROR_NAME.c_str());
       shm_unlink(SHM_DELAY.c_str());
   
       // int fd;
       // int rc = 1;
       // std::string posname = "/dev/input/event";
       // int test;
       // syscall(SYS_futex, &test, FUTEX_WAIT, 0, NULL, NULL, 0);
   
       // // seteuid(1000);
       // if(getuid() != 0){
       //     std::cout << "Need SUDO" << getuid() << std::endl;
       // }
       // for(int i=0;i<15;++i){
       //     std::string fdpos = posname + std::to_string(i);
       //     fd = open(fdpos.c_str(), O_RDONLY | O_NONBLOCK);
       //     if(fd < 0){
       //         std::cout << "not this" << fdpos << std::endl;
       //         close(fd);
       //         continue;
       //     }
       //     else{
       //         break;
       //     }
       // }
       // // fd = open("/dev/input/event1", O_RDONLY|O_NONBLOCK);
   
       // std::cout << "FD name: " << fd << std::endl;
       // rc = libevdev_new_from_fd(fd, &dev);
       // if (rc < 0){
       //     std::cout << "failed to init " << rc << std::endl;
       //     return -1;
       // }
       // std::cout << "device name: " << libevdev_get_name(dev) << std::endl;
       // std::cout << "bus type: " << libevdev_get_id_bustype(dev) << std::endl;
       // std::cout << "vendor id: " << libevdev_get_id_vendor(dev) << std::endl;
       // std::cout << "product id: " << libevdev_get_id_product(dev) << std::endl;
       // if(!libevdev_has_event_type(dev, EV_REL) ||
       //     !libevdev_has_event_code(dev, EV_KEY, BTN_LEFT)){
       //         std::cout << "this is not mouse" << std::endl;
       //     }
       // libevdev_free(dev);
       return 0;
       // std::cout << "" << std::endl;
   }
