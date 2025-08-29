
.. _program_listing_file_include_tests_INPUT_TESTS_linux_I_WAKER.cpp:

Program Listing for File linux_I_WAKER.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_INPUT_TESTS_linux_I_WAKER.cpp>` (``include\tests\INPUT_TESTS\linux_I_WAKER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "LINUX_INPUT.hpp"
   #include <chrono>
   #include <fcntl.h>
   #include <iostream>
   #include <linux/futex.h>
   #include <sys/mman.h>
   #include <sys/syscall.h>
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
   
   int
   main(int argc, char *argv[])
   {
   
       std::string SHM_FUTEX_NAME        = argv[1];
       std::string SHM_EVDEV_MIRROR_NAME = argv[2];
       std::string SHM_DELAY             = "/PDJE_DELAY_CHECK";
       int         delay_shm_fd = shm_open(SHM_DELAY.c_str(), O_RDWR, 0666);
       int         futex_shm_fd = shm_open(SHM_FUTEX_NAME.c_str(), O_RDWR, 0666);
       int evdev_shm = shm_open(SHM_EVDEV_MIRROR_NAME.c_str(), O_RDWR, 0666);
   
       int *futexVar = (int *)mmap(
           NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, futex_shm_fd, 0);
       int *tempstopper =
           (int *)mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, evdev_shm, 0);
       // int* evdevEventFD = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
       // MAP_SHARED, evdev_shm, 0);
   
       int                                         evdevfd;
       std::chrono::_V2::system_clock::time_point *delay_checker =
           (std::chrono::_V2::system_clock::time_point *)mmap(
               NULL,
               sizeof(std::chrono::_V2::system_clock::time_point),
               PROT_READ | PROT_WRITE,
               MAP_SHARED,
               delay_shm_fd,
               0);
       std::string posRoot = "/dev/input/event";
       libevdev   *dev     = nullptr;
       for (int i = 14; i > 0; --i) {
           std::string pos = posRoot + std::to_string(i);
           (evdevfd)       = open(pos.c_str(), O_RDONLY); // | O_NONBLOCK);
           std::cout << pos << std::endl;
           if (libevdev_new_from_fd((evdevfd), &dev) < 0) {
               std::cout << "init failed" << std::endl;
               close((evdevfd));
               continue;
           }
           if (libevdev_has_event_code(dev, EV_REL, REL_X) > 0) {
               break;
           } else {
               std::cout << "not a mouse" << std::endl;
               libevdev_free(dev);
               dev = nullptr;
               close(evdevfd);
           }
       }
       input_event ev;
   
       while (true) {
           (*delay_checker) = std::chrono::high_resolution_clock::now();
           futex_wake(futexVar);
           if (*tempstopper != 0) {
               break;
           }
           // sleep(1);
           int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_BLOCKING, &ev);
           if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
               auto code  = ev.code;
               auto time  = ev.time;
               auto type  = ev.type;
               auto value = ev.value;
               std::cout << "evdev IO Result: " << std::endl;
               std::cout << "code: " << code << std::endl;
               std::cout << "time: " << time.tv_usec << std::endl;
               std::cout << "type: " << type << std::endl;
               std::cout << "value: " << value << std::endl;
               std::cout << "WAKEUP CALL" << std::endl;
           } else {
               std::cout << "Failed" << std::endl;
           }
       }
       libevdev_free(dev);
       close(futex_shm_fd);
       close(evdev_shm);
       close(evdevfd);
       munmap(futexVar, sizeof(int));
       munmap(tempstopper, sizeof(int));
       // munmap(evdevEventFD, sizeof(int));
       munmap(delay_checker, sizeof(std::chrono::_V2::system_clock::time_point));
       shm_unlink(SHM_FUTEX_NAME.c_str());
       shm_unlink(SHM_EVDEV_MIRROR_NAME.c_str());
       shm_unlink(SHM_DELAY.c_str());
       return 0;
   }
