#pragma once

#include <string>
#include <vector>
#include <mutex>

#include <functional>

namespace errpdje{
    using ERR_MSG = std::string;
    using ERR_SOURCE = std::string;
    enum ERR_TYPE{
        MEM_NO_ALLOC,
        PTR_NO_EXISTS,
        DATA_SHOULD_HAVE_VALUE,
        SQL_ERROR
    };

    struct ERR_LOG{
        ERR_MSG message;
        ERR_TYPE etype;
        ERR_SOURCE eSource;
    };

    using ERR_CONTAINER = std::vector<ERR_LOG>;

    extern ERR_CONTAINER pdje_errors;
    extern std::mutex container_locker;
    //ereport is thread safe.
    extern void ereport(const ERR_MSG& errmsg, ERR_TYPE EType, const ERR_SOURCE& Source_From);

    extern ERR_LOG get_recent_err();
};

