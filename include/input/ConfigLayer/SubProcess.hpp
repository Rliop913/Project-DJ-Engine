#pragma once
#include "PDJE_Crypto.hpp"
#include "ipc_named_mutex.hpp"
#include "ipc_shared_memory.hpp"
using namespace PDJE_CRYPTO;
#include <string>
class SubProcCTRL {

  private:
  public:
    void
    OpenProcess(PSK                   &key,
                const fs::path        &memFirst,
                const PDJE_IPC::MNAME &firstLock,
                const fs::path        &memSecond,
                const PDJE_IPC::MNAME &secondLock);
    void
    CloseProcess();
    void
    AddBuffer();
    template <typename T, int MEM_PROT_FLAG>
    bool
    AddBuffer(const PDJE_IPC::SharedMem<T, MEM_PROT_FLAG> &mem,
              const std::string                           &mem_path,
              const std::string                           &dataType);
    SubProcCTRL();
    ~SubProcCTRL();
};