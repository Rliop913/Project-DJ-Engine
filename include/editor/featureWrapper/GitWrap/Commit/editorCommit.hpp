#pragma once
#include <git2.h>

#include <string>
#include <vector>
#include <optional>
namespace gitwrap
{
    struct commit{
        git_oid commitID;
        std::string msg;
    };

    using MAYBE_COMMIT = std::optional<commit>;

}
