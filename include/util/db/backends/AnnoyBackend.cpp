#include "AnnoyBackend.hpp"
#include "util/db/backends/detail/AnnoyManifest.hpp"

#include <annoy/annoylib.h>
#include <annoy/kissrandom.h>

#include <algorithm>
#include <cstdlib>
#include <limits>
#include <map>
#include <stdexcept>

namespace PDJE_UTIL::db::backends {
namespace {

void
validate_config(const AnnoyConfig &config)
{
    if (config.root_path.empty()) {
        throw std::invalid_argument("AnnoyConfig.root_path must not be empty.");
    }
    if (config.dimension == 0) {
        throw std::invalid_argument(
            "AnnoyConfig.dimension must be greater than zero.");
    }
    if (config.dimension >
            static_cast<std::size_t>(std::numeric_limits<int>::max()) ||
        config.dimension >
            std::numeric_limits<std::size_t>::max() / sizeof(float)) {
        throw std::invalid_argument("AnnoyConfig.dimension is too large.");
    }
    if (config.trees <= 0) {
        throw std::invalid_argument(
            "AnnoyConfig.trees must be greater than zero.");
    }
    if (config.open_options.read_only &&
        (config.open_options.create_if_missing ||
         config.open_options.truncate_if_exists)) {
        throw std::invalid_argument(
            "Annoy read-only mode cannot create or truncate the index.");
    }
}

} // namespace

class AnnoyBackend::Impl {
  public:
    using Index = Annoy::AnnoyIndex<int,
                                    float,
                                    Annoy::Angular,
                                    Annoy::Kiss32Random,
                                    Annoy::AnnoyIndexSingleThreadedBuildPolicy>;

    void
    require_open() const
    {
        if (!is_open)
            throw std::logic_error("Annoy backend is not open.");
    }
    void
    require_writable() const
    {
        require_open();
        if (config.open_options.read_only) {
            throw std::logic_error("Annoy backend is opened read-only.");
        }
    }

    void
    reset()
    {
        index.reset();
        ids.clear();
        records.clear();
        index_dirty   = true;
        storage_dirty = false;
        is_open       = false;
    }

    void
    load()
    {
        records       = detail::load_annoy_manifest(config);
        index_dirty   = true;
        storage_dirty = false;
    }

    void
    rebuild() const
    {
        require_open();
        if (!index_dirty)
            return;
        index = std::make_unique<Index>(static_cast<int>(config.dimension));
        ids.clear();
        int item_id = 0;
        for (const auto &[key, item] : records) {
            if (item.embedding.size() != config.dimension) {
                throw std::runtime_error(
                    "Annoy item dimension is inconsistent.");
            }
            char *error = nullptr;
            if (!index->add_item(item_id, item.embedding.data(), &error)) {
                std::string message = error ? error : "Annoy add_item failed.";
                std::free(error);
                throw std::runtime_error(message);
            }
            ids[item_id++] = key;
        }
        if (!records.empty()) {
            char *error = nullptr;
            if (!index->build(config.trees, -1, &error)) {
                std::string message = error ? error : "Annoy build failed.";
                std::free(error);
                throw std::runtime_error(message);
            }
        }
        index_dirty = false;
    }

    void
    flush()
    {
        require_open();
        if (config.open_options.read_only || !storage_dirty)
            return;
        detail::save_annoy_manifest_atomic(config, records);
        storage_dirty = false;
    }

    std::vector<Key>
    sorted_keys() const
    {
        std::vector<Key> output;
        output.reserve(records.size());
        for (const auto &[key, item] : records)
            output.push_back(key);
        std::sort(output.begin(), output.end());
        return output;
    }

    config_type                    config{};
    bool                           is_open       = false;
    mutable bool                   index_dirty   = true;
    bool                           storage_dirty = false;
    detail::AnnoyRecords           records;
    mutable std::map<int, Key>     ids;
    mutable std::unique_ptr<Index> index;
};

AnnoyBackend::AnnoyBackend() : impl_(std::make_unique<Impl>())
{
}
AnnoyBackend::~AnnoyBackend()                        = default;
AnnoyBackend::AnnoyBackend(AnnoyBackend &&) noexcept = default;
AnnoyBackend &
AnnoyBackend::operator=(AnnoyBackend &&other)
{
    if (this != &other) {
        if (impl_ && impl_->is_open)
            close();
        impl_ = std::move(other.impl_);
    }
    return *this;
}

void
AnnoyBackend::create(const config_type &config)
{
    validate_config(config);
    std::error_code error;
    std::filesystem::create_directories(config.root_path, error);
    if (error)
        throw std::runtime_error("Failed to create Annoy directory: " +
                                 error.message());
}

void
AnnoyBackend::destroy(const config_type &config)
{
    if (config.root_path.empty()) {
        throw std::invalid_argument("AnnoyConfig.root_path must not be empty.");
    }
    std::error_code error;
    std::filesystem::remove_all(config.root_path, error);
    if (error)
        throw std::runtime_error("Failed to remove Annoy directory: " +
                                 error.message());
}

void
AnnoyBackend::open(const config_type &config)
{
    if (!impl_)
        impl_ = std::make_unique<Impl>();
    if (impl_->is_open)
        throw std::logic_error("Annoy backend is already open.");
    validate_config(config);
    if (config.open_options.truncate_if_exists)
        destroy(config);
    if (!std::filesystem::exists(config.root_path)) {
        if (!config.open_options.create_if_missing) {
            throw std::out_of_range("Annoy backend directory does not exist.");
        }
        create(config);
    }
    impl_->config  = config;
    impl_->is_open = true;
    try {
        impl_->load();
    } catch (...) {
        impl_->reset();
        throw;
    }
}

void
AnnoyBackend::flush()
{
    if (!impl_)
        throw std::logic_error("Annoy backend is not open.");
    impl_->flush();
}

void
AnnoyBackend::close()
{
    if (!impl_ || !impl_->is_open)
        return;
    impl_->flush();
    impl_->reset();
}

bool
AnnoyBackend::contains(std::string_view key) const
{
    impl_->require_open();
    return impl_->records.contains(std::string(key));
}

nearest::Item
AnnoyBackend::get_item(std::string_view key) const
{
    impl_->require_open();
    const auto found = impl_->records.find(std::string(key));
    if (found == impl_->records.end())
        throw std::out_of_range("Annoy item was not found.");
    return found->second;
}

void
AnnoyBackend::upsert_item(const nearest::Item &item)
{
    impl_->require_writable();
    if (item.id.empty())
        throw std::invalid_argument("Annoy item id must not be empty.");
    if (item.embedding.size() != impl_->config.dimension) {
        throw std::invalid_argument(
            "Annoy item dimension does not match configuration.");
    }
    impl_->records[item.id] = item;
    impl_->index_dirty      = true;
    impl_->storage_dirty    = true;
}

void
AnnoyBackend::erase_item(std::string_view key)
{
    impl_->require_writable();
    impl_->records.erase(std::string(key));
    impl_->index_dirty   = true;
    impl_->storage_dirty = true;
}

std::vector<nearest::SearchHit>
AnnoyBackend::search(std::span<const float> query,
                     nearest::SearchOptions options) const
{
    impl_->require_open();
    if (query.size() != impl_->config.dimension) {
        throw std::invalid_argument(
            "Annoy query dimension does not match configuration.");
    }
    if (options.limit == 0 || impl_->records.empty())
        return {};
    impl_->rebuild();
    std::vector<int>   ids;
    std::vector<float> distances;
    impl_->index->get_nns_by_vector(
        query.data(), options.limit, options.search_k, &ids, &distances);
    std::vector<nearest::SearchHit> output;
    for (std::size_t index = 0; index < ids.size(); ++index) {
        const auto item = impl_->records.find(impl_->ids.at(ids[index]));
        if (item == impl_->records.end())
            continue;
        output.push_back({ item->second.id,
                           index < distances.size() ? distances[index] : 0.0F,
                           item->second.text_payload,
                           item->second.bytes_payload });
    }
    return output;
}

std::vector<Key>
AnnoyBackend::list_keys() const
{
    impl_->require_open();
    return impl_->sorted_keys();
}

} // namespace PDJE_UTIL::db::backends
