#include "RocksDbBackend.hpp"

#include <rocksdb/db.h>
#include <rocksdb/options.h>

#include <cstring>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace PDJE_UTIL::db::backends {

namespace {

rocksdb::Slice
slice_of(std::string_view value)
{
    return rocksdb::Slice(value.data(), value.size());
}

} // namespace

class RocksDbBackend::Impl {
  public:
    ~Impl()
    {
        delete db_;
    }

    common::Result<void>
    open(const config_type &cfg)
    {
        if (db_ != nullptr) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument,
                  "RocksDB backend is already open." });
        }
        if (cfg.path.empty()) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument,
                  "RocksDbConfig.path must not be empty." });
        }

        config_ = cfg;
        if (config_.open_options.truncate_if_exists) {
            auto destroyed = RocksDbBackend::destroy(config_);
            if (!destroyed.ok()) {
                return destroyed;
            }
        }

        const bool exists = std::filesystem::exists(config_.path);
        if (!exists && !config_.open_options.create_if_missing &&
            !config_.open_options.read_only) {
            return common::Result<void>::failure(
                { common::StatusCode::not_found,
                  "RocksDB directory does not exist." });
        }

        auto options              = db_options_;
        options.create_if_missing = config_.open_options.create_if_missing;
        options.error_if_exists   = false;

        rocksdb::Status status;
        if (config_.open_options.read_only) {
            status = rocksdb::DB::OpenForReadOnly(
                options, config_.path.string(), &db_);
        } else {
            status = rocksdb::DB::Open(options, config_.path.string(), &db_);
        }

        if (!status.ok()) {
            db_ = nullptr;
            return common::Result<void>::failure(
                { common::StatusCode::backend_error, status.ToString() });
        }

        read_options_  = {};
        write_options_ = {};
        return common::Result<void>::success();
    }

    common::Result<void>
    close()
    {
        delete db_;
        db_            = nullptr;
        config_        = {};
        db_options_    = {};
        read_options_  = {};
        write_options_ = {};
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
                { common::StatusCode::type_mismatch,
                  "RocksDB value is not stored as text." });
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
                { common::StatusCode::type_mismatch,
                  "RocksDB value is not stored as bytes." });
        }

        Bytes bytes(raw.value().size() - 1);
        if (!bytes.empty()) {
            std::memcpy(bytes.data(), raw.value().data() + 1, bytes.size());
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

        auto put_status =
            db_->Put(write_options_, slice_of(key), rocksdb::Slice(encoded));
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
        encoded.append(reinterpret_cast<const char *>(value.data()),
                       value.size_bytes());

        auto put_status =
            db_->Put(write_options_, slice_of(key), rocksdb::Slice(encoded));
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
                { common::StatusCode::backend_error,
                  delete_status.ToString() });
        }
        return common::Result<void>::success();
    }

    common::Result<std::vector<Key>>
    list_keys(std::string_view prefix) const
    {
        if (auto status = require_open(); !status.ok()) {
            return common::Result<std::vector<Key>>::failure(status);
        }

        std::vector<Key>                   keys;
        std::unique_ptr<rocksdb::Iterator> iterator(
            db_->NewIterator(read_options_));
        for (iterator->Seek(slice_of(prefix)); iterator->Valid();
             iterator->Next()) {
            const auto current = iterator->key().ToStringView();
            if (!prefix.empty() && !current.starts_with(prefix)) {
                break;
            }
            keys.emplace_back(current);
        }

        if (!iterator->status().ok()) {
            return common::Result<std::vector<Key>>::failure(
                { common::StatusCode::backend_error,
                  iterator->status().ToString() });
        }

        return common::Result<std::vector<Key>>::success(std::move(keys));
    }

    common::Status
    require_open() const
    {
        if (db_ == nullptr) {
            return { common::StatusCode::closed,
                     "RocksDB backend is not open." };
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
            return { common::StatusCode::unsupported,
                     "RocksDB backend is opened read-only." };
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
                { common::StatusCode::not_found,
                  "RocksDB key was not found." });
        }
        if (!get_status.ok()) {
            return common::Result<std::vector<char>>::failure(
                { common::StatusCode::backend_error, get_status.ToString() });
        }

        return common::Result<std::vector<char>>::success(
            std::vector<char>(value.begin(), value.end()));
    }

  private:
    config_type           config_{};
    rocksdb::Options      db_options_{};
    rocksdb::ReadOptions  read_options_{};
    rocksdb::WriteOptions write_options_{};
    rocksdb::DB          *db_ = nullptr;
};

RocksDbBackend::RocksDbBackend() : impl_(std::make_unique<Impl>())
{
}

RocksDbBackend::~RocksDbBackend() = default;

RocksDbBackend::RocksDbBackend(RocksDbBackend &&other) noexcept = default;

RocksDbBackend &
RocksDbBackend::operator=(RocksDbBackend &&other) noexcept = default;

common::Result<void>
RocksDbBackend::create(const config_type &cfg)
{
    if (cfg.path.empty()) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "RocksDbConfig.path must not be empty." });
    }

    rocksdb::Options options;
    options.create_if_missing = true;
    options.error_if_exists   = false;

    rocksdb::DB *db  = nullptr;
    auto open_status = rocksdb::DB::Open(options, cfg.path.string(), &db);
    if (!open_status.ok()) {
        return common::Result<void>::failure(
            { common::StatusCode::backend_error, open_status.ToString() });
    }

    delete db;
    return common::Result<void>::success();
}

common::Result<void>
RocksDbBackend::destroy(const config_type &cfg)
{
    if (cfg.path.empty()) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "RocksDbConfig.path must not be empty." });
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
RocksDbBackend::open(const config_type &cfg)
{
    if (impl_ == nullptr) {
        impl_ = std::make_unique<Impl>();
    }
    return impl_->open(cfg);
}

common::Result<void>
RocksDbBackend::close()
{
    if (impl_ == nullptr) {
        return common::Result<void>::success();
    }
    return impl_->close();
}

common::Result<bool>
RocksDbBackend::contains(std::string_view key) const
{
    if (impl_ == nullptr) {
        return common::Result<bool>::failure(
            { common::StatusCode::closed, "RocksDB backend is not open." });
    }
    return impl_->contains(key);
}

common::Result<Text>
RocksDbBackend::get_text(std::string_view key) const
{
    if (impl_ == nullptr) {
        return common::Result<Text>::failure(
            { common::StatusCode::closed, "RocksDB backend is not open." });
    }
    return impl_->get_text(key);
}

common::Result<Bytes>
RocksDbBackend::get_bytes(std::string_view key) const
{
    if (impl_ == nullptr) {
        return common::Result<Bytes>::failure(
            { common::StatusCode::closed, "RocksDB backend is not open." });
    }
    return impl_->get_bytes(key);
}

common::Result<void>
RocksDbBackend::put_text(std::string_view key, std::string_view value)
{
    if (impl_ == nullptr) {
        return common::Result<void>::failure(
            { common::StatusCode::closed, "RocksDB backend is not open." });
    }
    return impl_->put_text(key, value);
}

common::Result<void>
RocksDbBackend::put_bytes(std::string_view           key,
                          std::span<const std::byte> value)
{
    if (impl_ == nullptr) {
        return common::Result<void>::failure(
            { common::StatusCode::closed, "RocksDB backend is not open." });
    }
    return impl_->put_bytes(key, value);
}

common::Result<void>
RocksDbBackend::erase(std::string_view key)
{
    if (impl_ == nullptr) {
        return common::Result<void>::failure(
            { common::StatusCode::closed, "RocksDB backend is not open." });
    }
    return impl_->erase(key);
}

common::Result<std::vector<Key>>
RocksDbBackend::list_keys(std::string_view prefix) const
{
    if (impl_ == nullptr) {
        return common::Result<std::vector<Key>>::failure(
            { common::StatusCode::closed, "RocksDB backend is not open." });
    }
    return impl_->list_keys(prefix);
}

} // namespace PDJE_UTIL::db::backends
