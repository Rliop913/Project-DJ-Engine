#pragma once
#include "Parallel_Args.hpp"

namespace PDJE_PARALLEL {

class Backend {
  private:
    BACKEND_T backend_now;

  public:
    BACKEND_T
    PrintBackendType();

    bool
    LoadBackend();

    Backend();
    ~Backend();
};
}; // namespace PDJE_PARALLEL
