#pragma once

#include "global/PDJE_EXPORT_SETTER.hpp"
#include "util/db/DbTypes.hpp"
#include "util/db/nearest/Types.hpp"

#include <filesystem>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::db::backends {

struct AnnoyConfig {
    std::filesystem::path root_path;
    OpenOptions open_options {};
    std::size_t dimension = 0;
    int trees = 10;
    bool prefault = false;
};

class PDJE_API AnnoyBackend {
  public:
    using config_type = AnnoyConfig;

    AnnoyBackend();
    ~AnnoyBackend();
    AnnoyBackend(AnnoyBackend &&) noexcept;
    AnnoyBackend &operator=(AnnoyBackend &&) noexcept;
    AnnoyBackend(const AnnoyBackend &) = delete;
    AnnoyBackend &operator=(const AnnoyBackend &) = delete;

    static void create(const config_type &config);
    static void destroy(const config_type &config);
    void open(const config_type &config);
    void close();
    bool contains(std::string_view key) const;
    nearest::Item get_item(std::string_view key) const;
    void upsert_item(const nearest::Item &item);
    void erase_item(std::string_view key);
    std::vector<nearest::SearchHit> search(
        std::span<const float> query,
        nearest::SearchOptions options = {}) const;
    std::vector<Key> list_keys() const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace PDJE_UTIL::db::backends
