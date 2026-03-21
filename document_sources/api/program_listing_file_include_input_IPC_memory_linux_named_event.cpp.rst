
.. _program_listing_file_include_input_IPC_memory_linux_named_event.cpp:

Program Listing for File named_event.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_memory_linux_named_event.cpp>` (``include/input/IPC/memory/linux/named_event.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ipc_named_event.hpp"
   #include "ipc_shared_memory.hpp"
   #include <cerrno>
   #include <cstdint>
   #include <cstring>
   #include <ctime>
   #include <semaphore.h>
   #include <stdexcept>
   #include <string>
   
   namespace PDJE_IPC {
   
   void
   EVENT::ClientInit(const MNAME &name)
   {
       name_cache = posix_shmem_macro(name);
       hdlp       = sem_open(name_cache.c_str(), O_CREAT, 0666, 0);
       if (!hdlp || hdlp == SEM_FAILED) {
           critlog("failed to create ipc event. on linux.");
       }
   }
   void
   EVENT::HostInit(const MNAME &name)
   {
       name_cache = posix_shmem_macro(name);
       hdlp       = sem_open(name_cache.c_str(), O_CREAT, 0666, 0);
       if (!hdlp || hdlp == SEM_FAILED) {
           critlog("failed to create ipc event. on linux.");
       }
   }
   void
   EVENT::Wait(const uint16_t waittime_ms)
   {
       timespec ts{};
       if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
           critlog("clock get time failed on linux. replacing with wait "
                   "infinite.");
           Wait_Infinite();
           return;
       }
       auto addns = static_cast<long>(waittime_ms) * 1'000'000L;
       ts.tv_sec += addns / 1'000'000'000L;
       ts.tv_nsec += addns % 1'000'000'000L;
       if (ts.tv_nsec >= 1'000'000'000L) {
           ts.tv_sec += 1;
           ts.tv_nsec -= 1'000'000'000L;
       }
       for (;;) {
           if (sem_timedwait(reinterpret_cast<sem_t *>(hdlp), &ts) == 0) {
               return;
           }
           const int e = errno;
           if (e == ETIMEDOUT) {
               return;
           } else if (e == EINTR) {
               continue;
           } else {
               throw std::runtime_error("failed to wait ipc event." +
                                        std::string(strerror(e)));
           }
       }
   }
   
   void
   EVENT::Wait_Infinite()
   {
       for (;;) {
           if (sem_wait(reinterpret_cast<sem_t *>(hdlp)) == 0) {
               return;
           } else {
               const int e = errno;
               if (e == EINTR) {
                   continue;
               } else {
                   throw std::runtime_error("failed to wait ipc event infinite." +
                                            std::string(strerror(e)));
               }
           }
       }
   }
   
   void
   EVENT::Wake()
   {
       if (sem_post(reinterpret_cast<sem_t *>(hdlp)) == 0) {
           return;
       }
   
       const int e = errno;
       throw std::runtime_error(std::string("failed to wake ipc event: errno=") +
                                std::to_string(e) + " (" + std::strerror(e) + ")");
   }
   
   EVENT::~EVENT()
   {
       if (hdlp) {
           sem_close(reinterpret_cast<sem_t *>(hdlp));
       }
       sem_unlink(name_cache.c_str());
   }
   
   }; // namespace PDJE_IPC
