
.. _program_listing_file_include_input_--DEPRECATED-linux_RT_OneTimeSysSetup.cpp:

Program Listing for File OneTimeSysSetup.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_--DEPRECATED-linux_RT_OneTimeSysSetup.cpp>` (``include\input\--DEPRECATED-linux\RT\OneTimeSysSetup.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "OneTimeSysSetup.hpp"
   
   void
   OneTimeSysSetup::FixCPU(int core_number)
   {
       int valid_res = CoreValid(core_number);
       if (valid_res < 0) {
           switch (valid_res) {
           case PDJE_RT_ERROR::FAILED_TO_SCHED_GETAFFINITY:
               throw "failed to sched getaffinity";
           case PDJE_RT_ERROR::FAILED_TO_SET_CPU_NUMBER:
               throw "failed to set cpu number";
           }
       }
   
       cpu_set_t cpu_set;
       CPU_ZERO(&cpu_set);
   
       CPU_SET(core_number, &cpu_set);
   
       if (sched_setaffinity(0, sizeof(cpu_set), &cpu_set) != 0) {
           throw "failed to set affinity";
       }
   
       if (numa_available() != -1) {
           int node = numa_node_of_cpu(core_number);
           node     = node >= 0 ? node : 0;
   
           int policy = MPOL_BIND;
   
           bitmask *node_mask = numa_allocate_nodemask();
   
           if (!node_mask) {
               throw "failed to allocate node mask";
           } else {
               numa_bitmask_clearall(node_mask);
               numa_bitmask_setbit(node_mask, node);
   
               if (set_mempolicy(policy, node_mask->maskp, node_mask->size)) {
                   throw "set mem policy failed";
               }
               numa_free_nodemask(node_mask);
           }
       }
   }
   
   int
   OneTimeSysSetup::CoreValid(int core_number)
   {
       cpu_set_t allowed;
       CPU_ZERO(&allowed);
   
       if (sched_getaffinity(0, sizeof(allowed), &allowed) != 0) {
           return PDJE_RT_ERROR::FAILED_TO_SCHED_GETAFFINITY;
       }
   
       if (!CPU_ISSET(core_number, &allowed)) {
   
           for (int i = 0; i < CPU_SETSIZE; ++i) {
               if (CPU_ISSET(i, &allowed)) {
                   core_number = i;
                   break;
               }
           }
           if (!CPU_ISSET(core_number, &allowed)) {
               return PDJE_RT_ERROR::FAILED_TO_SET_CPU_NUMBER;
           }
       }
       return 0;
   }
   
   void
   OneTimeSysSetup::MLock()
   {
       mlockall(MCL_CURRENT | MCL_FUTURE);
   }
