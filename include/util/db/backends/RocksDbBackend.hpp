#pragma once

#include "util/common/Result.hpp"
#include "util/db/DbTypes.hpp"

#include <rocksdb/db.h>
#include <rocksdb/options.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::db::backends {

struct RocksDbConfig {
    std::filesystem::path path;
    OpenOptions           open_options {};
    rocksdb::Options      db_options {};
    rocksdb::ReadOptions  read_options {};
    rocksdb::WriteOptions write_options {};
};

class RocksDbBackend {
  public:
    using config_type = RocksDbConfig;

    static common::Result<void>
    create(const config_type &cfg)
    {
        if (cfg.path.empty()) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument, "RocksDbConfig.path must not be empty." });
        }

        auto options               = cfg.db_options;
        options.create_if_missing  = true;
        options.error_if_exists    = false;
        rocksdb::DB *db            = nullptr;
        auto         create_status = rocksdb::DB::Open(options, cfg.path.string(), &db);
        if (!create_status.ok()) {
            return common::Result<void>::failure(
                { common::StatusCode::backend_error, create_status.ToString() });
        }
        delete db;
        return common::Result<void>::success();
    }

    static common::Result<void>
    destroy(const config_type &cfg)
    {
        if (cfg.path.empty()) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument, "RocksDbConfig.path must not be empty." });
        }

        std::error_code ec;
        std::filesystem::remove_all(cfg.path, ec);
        if (ec) {
            return common::Result<void>::failure(
                { common::StatusCode::io_error, ec.message() });
        }
        return common::Result<void>::success();
    }

    common::Result<void>
    open(const config_type &cfg)
    {
        if (db_ != nullptr) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument, "RocksDB backend is already open." });
        }
        if (cfg.path.empty()) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument, "RocksDbConfig.path must not be empty." });
        }

        config_ = cfg;
        if (config_.open_options.truncate_if_exists) {
            auto destroyed = destroy(config_);
            if (!destroyed.ok()) {
                return destroyed;
            }
        }

        const bool exists = std::filesystem::exists(config_.path);
        if (!exists && !config_.open_options.create_if_missing &&
            !config_.open_options.read_only) {
            return common::Result<void>::failure(
                { common::StatusCode::not_found, "RocksDB directory does not exist." });
        }

        auto options               = config_.db_options;
        options.create_if_missing  = config_.open_options.create_if_missing;
        options.error_if_exists    = false;

        rocksdb::Status status;
        if (config_.open_options.read_only) {
            status = rocksdb::DB::OpenForReadOnly(options, config_.path.string(), &db_);
        } else {
            status = rocksdb::DB::Open(options, config_.path.string(), &db_);
        }

        if (!status.ok()) {
            db_ = nullptr;
            return common::Result<void>::failure(
                { common::StatusCode::backend_error, status.ToString() });
        }

        read_options_  = config_.read_options;
        write_options_ = config_.write_options;
        return common::Result<void>::success();
    }

    common::Result<void>
    close()
    {
        delete db_;
        db_ = nullptr;
        return common::Result<void>::success();
    }

    common::Result<bool>
    contains(std::string_view key) const
    {
        if (auto status = require_open(); !status.ok()) {
            return common::Result<bool>::failure(status);
        }

        std::string value;
        auto        get_status = db_->Get(read_options_, slice_of(key), &value);
        if (get_status.IsNotFound()) {
            return common::Result<bool>::success(false);
        }
        if (!get_status.ok()) {
            return common::Result<bool>::failure(
                { common::StatusCode::backend_error, get_status.ToString() });
        }
        return common::Result<bool>::success(true);
    }

    common::Result<Text>
    get_text(std::string_view key) const
    {
        auto raw = get_raw(key);
        if (!raw.ok()) {
            return common::Result<Text>::failure(raw.status());
        }
        if (raw.value().empty() || raw.value().front() != 'T') {
            return common::Result<Text>::failure(
                { common::StatusCode::type_mismatch, "RocksDB value is not stored as text." });
        }
        return common::Result<Text>::success(
            std::string(raw.value().begin() + 1, raw.value().end()));
    }

    common::Result<Bytes>
    get_bytes(std::string_view key) const
    {
        auto raw = get_raw(key);
        if (!raw.ok()) {
            return common::Result<Bytes>::failure(raw.status());
        }
        if (raw.value().empty() || raw.value().front() != 'B') {
            return common::Result<Bytes>::failure(
                { common::StatusCode::type_mismatch, "RocksDB value is not stored as bytes." });
        }

        Bytes bytes(raw.value().size() - 1);
        if (!bytes.empty()) {
            std::memcpy(bytes.data(),
                        raw.value().data() + 1,
                        bytes.size());
        }
        return common::Result<Bytes>::success(std::move(bytes));
    }

    common::Result<void>
    put_text(std::string_view key, std::string_view value)
    {
        if (auto status = require_writable(); !status.ok()) {
            return common::Result<void>::failure(status);
        }

        std::string encoded;
        encoded.reserve(value.size() + 1);
        encoded.push_back('T');
        encoded.append(value.data(), value.size());

        auto put_status = db_->Put(write_options_, slice_of(key), rocksdb::Slice(encoded));
        if (!put_status.ok()) {
            return common::Result<void>::failure(
                { common::StatusCode::backend_error, put_status.ToString() });
        }
        return common::Result<void>::success();
    }

    common::Result<void>
    put_bytes(std::string_view key, std::span<const std::byte> value)
    {
        if (auto status = require_writable(); !status.ok()) {
            return common::Result<void>::failure(status);
        }

        std::string encoded;
        encoded.reserve(value.size_bytes() + 1);
        encoded.push_back('B');
        encoded.append(reinterpret_cast<const char *>(value.data()), value.size_bytes());

        auto put_status = db_->Put(write_options_, slice_of(key), rocksdb::Slice(encoded));
        if (!put_status.ok()) {
            return common::Result<void>::failure(
                { common::StatusCode::backend_error, put_status.ToString() });
        }
        return common::Result<void>::success();
    }

    common::Result<void>
    erase(std::string_view key)
    {
        if (auto status = require_writable(); !status.ok()) {
            return common::Result<void>::failure(status);
        }

        auto delete_status = db_->Delete(write_options_, slice_of(key));
        if (!delete_status.ok()) {
            return common::Result<void>::failure(
                { common::StatusCode::backend_error, delete_status.ToString() });
        }
        return common::Result<void>::success();
    }

    common::Result<std::vector<Key>>
    list_keys(std::string_view prefix = {}) const
    {
        if (auto status = require_open(); !status.ok()) {
            return common::Result<std::vector<Key>>::failure(status);
        }

        std::vector<Key> keys;
        std::unique_ptr<rocksdb::Iterator> iterator(db_->NewIterator(read_options_));
        for (iterator->Seek(slice_of(prefix)); iterator->Valid(); iterator->Next()) {
            const auto current = iterator->key().ToStringView();
            if (!prefix.empty() && !current.starts_with(prefix)) {
                break;
            }
            keys.emplace_back(current);
        }

        if (!iterator->status().ok()) {
            return common::Result<std::vector<Key>>::failure(
                { common::StatusCode::backend_error, iterator->status().ToString() });
        }

        return common::Result<std::vector<Key>>::success(std::move(keys));
    }

  private:
    static rocksdb::Slice
    slice_of(std::string_view value)
    {
        return rocksdb::Slice(value.data(), value.size());
    }

    common::Status
    require_open() const
    {
        if (db_ == nullptr) {
            return { common::StatusCode::closed, "RocksDB backend is not open." };
        }
        return {};
    }

    common::Status
    require_writable() const
    {
        if (auto status = require_open(); !status.ok()) {
            return status;
        }
        if (config_.open_options.read_only) {
            return { common::StatusCode::unsupported, "RocksDB backend is opened read-only." };
        }
        return {};
    }

    common::Result<std::vector<char>>
    get_raw(std::string_view key) const
    {
        if (auto status = require_open(); !status.ok()) {
            return common::Result<std::vector<char>>::failure(status);
        }

        std::string value;
        auto        get_status = db_->Get(read_options_, slice_of(key), &value);
        if (get_status.IsNotFound()) {
            return common::Result<std::vector<char>>::failure(
                { common::StatusCode::not_found, "RocksDB key was not found." });
        }
        if (!get_status.ok()) {
            return common::Result<std::vector<char>>::failure(
                { common::StatusCode::backend_error, get_status.ToString() });
        }

        return common::Result<std::vector<char>>::success(
            std::vector<char>(value.begin(), value.end()));
    }

    config_type           config_ {};
    rocksdb::DB          *db_ = nullptr;
    rocksdb::ReadOptions  read_options_ {};
    rocksdb::WriteOptions write_options_ {};
};

} // namespace PDJE_UTIL::db::backends
