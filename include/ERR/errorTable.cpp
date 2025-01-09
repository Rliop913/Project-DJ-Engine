#include "errorTable.hpp"

namespace errpdje{
    ERR_CONTAINER pdje_errors;
    std::mutex container_locker;
    void
    ereport(const ERR_MSG& errmsg, ERR_TYPE EType, const ERR_SOURCE& Source_From)
    {
        container_locker.lock();
        ERR_LOG templog;
        templog.message = errmsg;
        templog.etype = EType;
        templog.eSource = Source_From;
        pdje_errors.emplace_back(templog);
        container_locker.unlock();
    }

    ERR_LOG
    get_recent_err()
    {
        return pdje_errors.back();
    }

}