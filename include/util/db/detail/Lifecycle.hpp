#pragma once

#include "util/common/Result.hpp"

#include <utility>

namespace PDJE_UTIL::db::detail {

template <class Backend>
common::Result<void>
open_backend(Backend                       &backend,
             bool                          &is_open,
             const typename Backend::config_type &cfg)
{
    auto opened = backend.open(cfg);
    if (opened.ok()) {
        is_open = true;
    }
    return opened;
}

template <class Backend>
common::Result<void>
close_if_open(Backend &backend, bool &is_open)
{
    if (!is_open) {
        return common::Result<void>::success();
    }

    auto closed = backend.close();
    if (closed.ok()) {
        is_open = false;
    }
    return closed;
}

template <class Backend>
void
take_backend_state(Backend &backend,
                   bool    &is_open,
                   Backend &&other_backend,
                   bool    &other_is_open) noexcept
{
    backend = std::move(other_backend);
    is_open = std::exchange(other_is_open, false);
}

} // namespace PDJE_UTIL::db::detail
