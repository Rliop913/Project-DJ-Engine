#pragma once

#include "global/PDJE_EXPORT_SETTER.hpp"
#include "util/db/DbTypes.hpp"

#include <filesystem>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::db::backends {

struct RocksDbConfig {
    std::filesystem::path path;
    OpenOptions           open_options{};
};

class PDJE_API RocksDbBackend {
  public:
    using config_type = RocksDbConfig;

    RocksDbBackend();
    ~RocksDbBackend();
    RocksDbBackend(RocksDbBackend &&) noexcept;
    RocksDbBackend &
    operator=(RocksDbBackend &&) noexcept;
    RocksDbBackend(const RocksDbBackend &) = delete;
    RocksDbBackend &
    operator=(const RocksDbBackend &) = delete;

    static void
    create(const config_type &config);
    static void
    destroy(const config_type &config);
    void
    open(const config_type &config);
    void
    close();
    bool
    contains(std::string_view key) const;
    Text
    get_text(std::string_view key) const;
    Bytes
    get_bytes(std::string_view key) const;
    void
    put_text(std::string_view key, std::string_view value);
    void
    put_bytes(std::string_view key, std::span<const std::byte> value);
    void
    erase(std::string_view key);
    std::vector<Key>
    list_keys(std::string_view prefix = {}) const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace PDJE_UTIL::db::backends
