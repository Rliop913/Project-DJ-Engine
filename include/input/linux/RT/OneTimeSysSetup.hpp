#pragma once
#include "Common_Features.hpp"
#include <numa.h>
#include <numaif.h>
#include <sched.h>
#include <sys/mman.h>
class OneTimeSysSetup {
  private:
    int
    CoreValid(int core_number);
    void
    FixCPU(int core_number = 2);
    void
    MLock();

  public:
    OneTimeSysSetup()
    {
        FixCPU();
        MLock();
    }
    ~OneTimeSysSetup()
    {
        munlockall();
    }
};