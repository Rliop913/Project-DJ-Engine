#pragma once

#include "global/PDJE_LOG_SETTER.hpp"
#include "util/db/detail/Lifecycle.hpp"
#include "util/db/nearest/BackendConcept.hpp"

#include <exception>
#include <type_traits>
#include <utility>

namespace PDJE_UTIL::db::nearest {

template <NearestNeighborBackendConcept Backend> class NearestNeighborIndex {
  public:
    using backend_type = Backend;
    using config_type  = typename Backend::config_type;

    Backend backend{};
    bool    is_open = false;

    static void
    create(const config_type &config)
    {
        Backend::create(config);
    }
    static void
    destroy(const config_type &config)
    {
        Backend::destroy(config);
    }

    static NearestNeighborIndex
    open(const config_type &config)
    {
        NearestNeighborIndex index;
        detail::open_backend(index.backend, index.is_open, config);
        return index;
    }

    NearestNeighborIndex() = default;
    NearestNeighborIndex(NearestNeighborIndex &&other) noexcept(
        std::is_nothrow_move_constructible_v<Backend>)
        : backend(std::move(other.backend)),
          is_open(std::exchange(other.is_open, false))
    {
    }
    NearestNeighborIndex &
    operator=(NearestNeighborIndex &&other) noexcept(
        std::is_nothrow_move_assignable_v<Backend>)
    {
        if (this != &other) {
            backend = std::move(other.backend);
            is_open = std::exchange(other.is_open, false);
        }
        return *this;
    }
    NearestNeighborIndex(const NearestNeighborIndex &) = delete;
    NearestNeighborIndex &
    operator=(const NearestNeighborIndex &) = delete;
    ~NearestNeighborIndex() {
        try{
            flush();
            close();
        }catch(const std::exception& e){
            critlog("error on close NearsetNeighborIndex. WHAT= ");
            critlog(e.what());
        }
    }

    void
    flush()
    {
        backend.flush();
    }
    void
    close()
    {
        detail::close_if_open(backend, is_open);
    }
    bool
    contains(std::string_view id) const
    {
        return backend.contains(id);
    }
    Item
    get_item(std::string_view id) const
    {
        return backend.get_item(id);
    }
    void
    upsert_item(const Item &item)
    {
        backend.upsert_item(item);
    }
    void
    erase_item(std::string_view id)
    {
        backend.erase_item(id);
    }
    std::vector<SearchHit>
    search(std::span<const float> query, SearchOptions options = {}) const
    {
        return backend.search(query, options);
    }
    std::vector<Key>
    list_keys() const
    {
        return backend.list_keys();
    }
};

} // namespace PDJE_UTIL::db::nearest
