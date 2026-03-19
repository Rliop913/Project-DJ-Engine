#pragma once

#include "util/common/Result.hpp"
#include "util/db/DbTypes.hpp"

#include <cstddef>
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

class RocksDbBackend {
  public:
    using config_type = RocksDbConfig;

    RocksDbBackend();
    ~RocksDbBackend();

    RocksDbBackend(RocksDbBackend &&other) noexcept;
    RocksDbBackend &
    operator=(RocksDbBackend &&other) noexcept;

    RocksDbBackend(const RocksDbBackend &) = delete;
    RocksDbBackend &
    operator=(const RocksDbBackend &) = delete;

    static common::Result<void>
    create(const config_type &cfg);

    static common::Result<void>
    destroy(const config_type &cfg);

    common::Result<void>
    open(const config_type &cfg);

    common::Result<void>
    close();

    common::Result<bool>
    contains(std::string_view key) const;

    common::Result<Text>
    get_text(std::string_view key) const;

    common::Result<Bytes>
    get_bytes(std::string_view key) const;

    common::Result<void>
    put_text(std::string_view key, std::string_view value);

    common::Result<void>
    put_bytes(std::string_view key, std::span<const std::byte> value);

    common::Result<void>
    erase(std::string_view key);

    common::Result<std::vector<Key>>
    list_keys(std::string_view prefix = {}) const;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace PDJE_UTIL::db::backends
