#pragma once
#include <PDJE_LOG_SETTER.hpp>
#include <filesystem>
#include <string>
namespace PDJE_IPC {
using MNAME = std::filesystem::path;
class MUTEX {
  private:
    void *mutex_handle = nullptr;

  public:
    bool
    lock();
    void
    unlock();
    void
    init(const MNAME &name);
    MUTEX(const MUTEX &) = delete;
    MUTEX &
    operator=(const MUTEX &) = delete;
    MUTEX();
    ~MUTEX();
};

class SCOPE_LOCK {
  private:
    bool locked = false;

  public:
    MUTEX *MP;
    SCOPE_LOCK(const SCOPE_LOCK &) = delete;
    SCOPE_LOCK &
    operator=(const SCOPE_LOCK &) = delete;
    SCOPE_LOCK(SCOPE_LOCK &&)     = delete;
    SCOPE_LOCK &
    operator=(SCOPE_LOCK &&) = delete;
    SCOPE_LOCK(MUTEX &M)
    {
        MP = &M;
        if (!M.lock()) {
            locked = false;
            critlog("mutex lock failed.");
        } else {
            locked = true;
        }
    }
    ~SCOPE_LOCK()
    {
        if (locked) {
            try {

                MP->unlock();
            } catch (const std::exception &e) {
                critlog("failed to unlock. What: ");
                critlog(e.what());
            }
        }
    }
};

}; // namespace PDJE_IPC