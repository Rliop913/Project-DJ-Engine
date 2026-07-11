#pragma once

#include <utility>

namespace PDJE_UTIL::db::detail {

template <class Backend, class Config>
void open_backend(Backend &backend, bool &is_open, const Config &config)
{
    backend.open(config);
    is_open = true;
}

template <class Backend>
void close_if_open(Backend &backend, bool &is_open)
{
    if (!is_open) {
        return;
    }
    backend.close();
    is_open = false;
}

template <class Backend>
void take_backend_state(Backend &backend,
                        bool &is_open,
                        Backend &&other_backend,
                        bool &other_is_open) noexcept
{
    backend = std::move(other_backend);
    is_open = std::exchange(other_is_open, false);
}

} // namespace PDJE_UTIL::db::detail
