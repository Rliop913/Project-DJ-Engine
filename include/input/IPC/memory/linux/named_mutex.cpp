#include "PDJE_LOG_SETTER.hpp"
#include "ipc_named_mutex.hpp"
#include "ipc_shared_memory.hpp"
#include <fcntl.h>
#include <semaphore.h>

namespace PDJE_IPC {

MUTEX::MUTEX()
{
    startlog();
}

void
MUTEX::init(const MNAME &name)
{
    mutex_name = posix_shmem_macro(name);

    mutex_handle = sem_open(mutex_name.c_str(), O_CREAT, 0666, 1);
    if (!mutex_handle || mutex_handle == SEM_FAILED) {
        critlog("failed to create ipc mutex. on linux.");
    }
}
bool
MUTEX::lock()
{
    int rc = sem_wait(reinterpret_cast<sem_t *>(mutex_handle));
    return rc == 0;
}

void
MUTEX::unlock()
{
    auto rc = sem_post(reinterpret_cast<sem_t *>(mutex_handle));
    if (rc != 0) {
        critlog("linux named mutex unlock caused error. Error code: ");
        critlog(rc);
    }
}

MUTEX::~MUTEX()
{
    if (mutex_handle) {
        sem_close(reinterpret_cast<sem_t *>(mutex_handle));
    }
    sem_unlink(mutex_name.c_str());
}
}; // namespace PDJE_IPC