#include "RocksDbBackend.hpp"

#include <rocksdb/db.h>
#include <rocksdb/options.h>

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

namespace PDJE_UTIL::db::backends {
namespace {

rocksdb::Slice
slice_of(std::string_view value)
{
    return { value.data(), value.size() };
}

void
require_ok(const rocksdb::Status &status, std::string_view context)
{
    if (!status.ok()) {
        throw std::runtime_error(std::string(context) + ": " +
                                 status.ToString());
    }
}

} // namespace

class RocksDbBackend::Impl {
  public:
    ~Impl()
    {
        delete db;
    }

    void
    require_open() const
    {
        if (db == nullptr) {
            throw std::logic_error("RocksDB backend is not open.");
        }
    }

    void
    require_writable() const
    {
        require_open();
        if (config.open_options.read_only) {
            throw std::logic_error("RocksDB backend is opened read-only.");
        }
    }

    std::string
    get_raw(std::string_view key) const
    {
        require_open();
        std::string value;
        const auto  status = db->Get(read_options, slice_of(key), &value);
        if (status.IsNotFound()) {
            throw std::out_of_range("RocksDB key was not found.");
        }
        require_ok(status, "RocksDB read failed");
        return value;
    }

    config_type           config{};
    rocksdb::ReadOptions  read_options{};
    rocksdb::WriteOptions write_options{};
    rocksdb::DB          *db = nullptr;
};

RocksDbBackend::RocksDbBackend() : impl_(std::make_unique<Impl>())
{
}
RocksDbBackend::~RocksDbBackend()                          = default;
RocksDbBackend::RocksDbBackend(RocksDbBackend &&) noexcept = default;
RocksDbBackend &
RocksDbBackend::operator=(RocksDbBackend &&) noexcept = default;

void
RocksDbBackend::create(const config_type &config)
{
    if (config.path.empty()) {
        throw std::invalid_argument("RocksDbConfig.path must not be empty.");
    }
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::DB *database     = nullptr;
    require_ok(rocksdb::DB::Open(options, config.path.string(), &database),
               "Failed to create RocksDB database");
    delete database;
}

void
RocksDbBackend::destroy(const config_type &config)
{
    if (config.path.empty()) {
        throw std::invalid_argument("RocksDbConfig.path must not be empty.");
    }
    std::error_code error;
    std::filesystem::remove_all(config.path, error);
    if (error) {
        throw std::runtime_error("Failed to remove RocksDB database: " +
                                 error.message());
    }
}

void
RocksDbBackend::open(const config_type &config)
{
    if (!impl_) {
        impl_ = std::make_unique<Impl>();
    }
    if (impl_->db != nullptr) {
        throw std::logic_error("RocksDB backend is already open.");
    }
    if (config.path.empty()) {
        throw std::invalid_argument("RocksDbConfig.path must not be empty.");
    }
    if (config.open_options.read_only &&
        (config.open_options.create_if_missing ||
         config.open_options.truncate_if_exists)) {
        throw std::invalid_argument(
            "RocksDB read-only mode cannot create or truncate the database.");
    }

    if (config.open_options.truncate_if_exists) {
        destroy(config);
    }
    if (!std::filesystem::exists(config.path) &&
        !config.open_options.create_if_missing) {
        throw std::out_of_range("RocksDB directory does not exist.");
    }

    rocksdb::Options options;
    options.create_if_missing = config.open_options.create_if_missing;
    rocksdb::DB    *database  = nullptr;
    rocksdb::Status status =
        config.open_options.read_only
            ? rocksdb::DB::OpenForReadOnly(
                  options, config.path.string(), &database)
            : rocksdb::DB::Open(options, config.path.string(), &database);
    require_ok(status, "Failed to open RocksDB database");
    impl_->config = config;
    impl_->db     = database;
}

void
RocksDbBackend::close()
{
    if (!impl_ || impl_->db == nullptr) {
        return;
    }
    delete impl_->db;
    impl_->db     = nullptr;
    impl_->config = {};
}

bool
RocksDbBackend::contains(std::string_view key) const
{
    impl_->require_open();
    std::string value;
    const auto  status =
        impl_->db->Get(impl_->read_options, slice_of(key), &value);
    if (status.IsNotFound()) {
        return false;
    }
    require_ok(status, "RocksDB contains failed");
    return true;
}

Text
RocksDbBackend::get_text(std::string_view key) const
{
    auto value = impl_->get_raw(key);
    if (value.empty() || value.front() != 'T') {
        throw std::logic_error("RocksDB value is not stored as text.");
    }
    return value.substr(1);
}

Bytes
RocksDbBackend::get_bytes(std::string_view key) const
{
    const auto value = impl_->get_raw(key);
    if (value.empty() || value.front() != 'B') {
        throw std::logic_error("RocksDB value is not stored as bytes.");
    }
    Bytes bytes(value.size() - 1);
    if (!bytes.empty()) {
        std::memcpy(bytes.data(), value.data() + 1, bytes.size());
    }
    return bytes;
}

void
RocksDbBackend::put_text(std::string_view key, std::string_view value)
{
    impl_->require_writable();
    std::string encoded("T");
    encoded.append(value);
    require_ok(impl_->db->Put(impl_->write_options, slice_of(key), encoded),
               "RocksDB text write failed");
}

void
RocksDbBackend::put_bytes(std::string_view           key,
                          std::span<const std::byte> value)
{
    impl_->require_writable();
    std::string encoded("B");
    encoded.append(reinterpret_cast<const char *>(value.data()), value.size());
    require_ok(impl_->db->Put(impl_->write_options, slice_of(key), encoded),
               "RocksDB byte write failed");
}

void
RocksDbBackend::erase(std::string_view key)
{
    impl_->require_writable();
    require_ok(impl_->db->Delete(impl_->write_options, slice_of(key)),
               "RocksDB erase failed");
}

std::vector<Key>
RocksDbBackend::list_keys(std::string_view prefix) const
{
    impl_->require_open();
    std::vector<Key>                   keys;
    std::unique_ptr<rocksdb::Iterator> iterator(
        impl_->db->NewIterator(impl_->read_options));
    for (iterator->Seek(slice_of(prefix)); iterator->Valid();
         iterator->Next()) {
        const auto key = iterator->key().ToStringView();
        if (!prefix.empty() && !key.starts_with(prefix)) {
            break;
        }
        keys.emplace_back(key);
    }
    require_ok(iterator->status(), "RocksDB key scan failed");
    return keys;
}

} // namespace PDJE_UTIL::db::backends
