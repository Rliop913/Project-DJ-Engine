#include "AnnoyBackend.hpp"

#include <annoy/annoylib.h>
#include <annoy/kissrandom.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace PDJE_UTIL::db::backends {
namespace {

std::string encode(std::span<const std::byte> input)
{
    static constexpr char digits[] = "0123456789abcdef";
    std::string output(input.size() * 2, '0');
    for (std::size_t index = 0; index < input.size(); ++index) {
        const auto value = std::to_integer<unsigned int>(input[index]);
        output[index * 2] = digits[value >> 4];
        output[index * 2 + 1] = digits[value & 0x0f];
    }
    return output;
}

std::string encode(std::string_view input)
{
    return encode({ reinterpret_cast<const std::byte *>(input.data()), input.size() });
}

int nibble(char value)
{
    if (value >= '0' && value <= '9') return value - '0';
    if (value >= 'a' && value <= 'f') return value - 'a' + 10;
    if (value >= 'A' && value <= 'F') return value - 'A' + 10;
    return -1;
}

std::vector<std::byte> decode(std::string_view input)
{
    if ((input.size() % 2) != 0) {
        throw std::runtime_error("Annoy manifest contains invalid hexadecimal data.");
    }
    std::vector<std::byte> output;
    output.reserve(input.size() / 2);
    for (std::size_t index = 0; index < input.size(); index += 2) {
        const int high = nibble(input[index]);
        const int low = nibble(input[index + 1]);
        if (high < 0 || low < 0) {
            throw std::runtime_error("Annoy manifest contains invalid hexadecimal data.");
        }
        output.push_back(static_cast<std::byte>((high << 4) | low));
    }
    return output;
}

std::string decode_text(std::string_view input)
{
    const auto bytes = decode(input);
    return { reinterpret_cast<const char *>(bytes.data()), bytes.size() };
}

nearest::Embedding decode_embedding(std::string_view input,
                                    std::size_t dimension)
{
    const auto bytes = decode(input);
    if (bytes.size() != dimension * sizeof(float)) {
        throw std::runtime_error("Annoy manifest embedding dimension is invalid.");
    }
    nearest::Embedding output(dimension);
    std::memcpy(output.data(), bytes.data(), bytes.size());
    return output;
}

std::vector<std::string> split_fields(const std::string &line)
{
    std::vector<std::string> fields;
    std::istringstream stream(line);
    for (std::string field; std::getline(stream, field, '\t');) {
        fields.push_back(std::move(field));
    }
    if (!line.empty() && line.back() == '\t') fields.emplace_back();
    return fields;
}

void validate_config(const AnnoyConfig &config)
{
    if (config.root_path.empty()) {
        throw std::invalid_argument("AnnoyConfig.root_path must not be empty.");
    }
    if (config.dimension == 0) {
        throw std::invalid_argument("AnnoyConfig.dimension must be greater than zero.");
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

    void require_open() const
    {
        if (!is_open) throw std::logic_error("Annoy backend is not open.");
    }
    void require_writable() const
    {
        require_open();
        if (config.open_options.read_only) {
            throw std::logic_error("Annoy backend is opened read-only.");
        }
    }

    void reset()
    {
        index.reset();
        ids.clear();
        records.clear();
        dirty = true;
        is_open = false;
    }

    void load()
    {
        const auto path = config.root_path / "records.tsv";
        if (!std::filesystem::exists(path)) return;
        std::ifstream input(path);
        if (!input) throw std::runtime_error("Failed to open Annoy manifest file.");
        for (std::string line; std::getline(input, line);) {
            if (line.empty()) continue;
            const auto fields = split_fields(line);
            if (fields.size() != 6) {
                throw std::runtime_error("Annoy manifest is malformed.");
            }
            nearest::Item item {
                .id = decode_text(fields[0]),
                .embedding = decode_embedding(fields[1], config.dimension)
            };
            if (fields[2] == "1") item.text_payload = decode_text(fields[3]);
            else if (fields[2] != "0") throw std::runtime_error("Invalid Annoy text flag.");
            if (fields[4] == "1") item.bytes_payload = decode(fields[5]);
            else if (fields[4] != "0") throw std::runtime_error("Invalid Annoy bytes flag.");
            records[item.id] = std::move(item);
        }
    }

    void rebuild() const
    {
        require_open();
        if (!dirty) return;
        index = std::make_unique<Index>(static_cast<int>(config.dimension));
        ids.clear();
        int item_id = 0;
        for (const auto &[key, item] : records) {
            if (item.embedding.size() != config.dimension) {
                throw std::runtime_error("Annoy item dimension is inconsistent.");
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
        dirty = false;
    }

    void persist() const
    {
        rebuild();
        std::ofstream output(config.root_path / "records.tsv", std::ios::trunc);
        if (!output) throw std::runtime_error("Failed to write Annoy manifest file.");
        for (const auto &key : sorted_keys()) {
            const auto &item = records.at(key);
            const auto embedding = std::span(
                reinterpret_cast<const std::byte *>(item.embedding.data()),
                item.embedding.size() * sizeof(float));
            output << encode(item.id) << '\t' << encode(embedding) << '\t'
                   << (item.text_payload ? "1\t" + encode(*item.text_payload) : "0\t")
                   << '\t'
                   << (item.bytes_payload ? "1\t" + encode(*item.bytes_payload) : "0\t")
                   << '\n';
        }
        const auto index_path = config.root_path / "index.ann";
        if (records.empty()) {
            std::error_code ignored;
            std::filesystem::remove(index_path, ignored);
            return;
        }
        char *error = nullptr;
        if (!index->save(index_path.string().c_str(), config.prefault, &error)) {
            std::string message = error ? error : "Annoy save failed.";
            std::free(error);
            throw std::runtime_error(message);
        }
    }

    std::vector<Key> sorted_keys() const
    {
        std::vector<Key> output;
        output.reserve(records.size());
        for (const auto &[key, item] : records) output.push_back(key);
        std::sort(output.begin(), output.end());
        return output;
    }

    config_type config {};
    bool is_open = false;
    mutable bool dirty = true;
    std::unordered_map<Key, nearest::Item> records;
    mutable std::map<int, Key> ids;
    mutable std::unique_ptr<Index> index;
};

AnnoyBackend::AnnoyBackend() : impl_(std::make_unique<Impl>()) {}
AnnoyBackend::~AnnoyBackend()
{
    try { close(); } catch (...) {}
}
AnnoyBackend::AnnoyBackend(AnnoyBackend &&) noexcept = default;
AnnoyBackend &AnnoyBackend::operator=(AnnoyBackend &&) noexcept = default;

void AnnoyBackend::create(const config_type &config)
{
    validate_config(config);
    std::error_code error;
    std::filesystem::create_directories(config.root_path, error);
    if (error) throw std::runtime_error("Failed to create Annoy directory: " + error.message());
}

void AnnoyBackend::destroy(const config_type &config)
{
    if (config.root_path.empty()) {
        throw std::invalid_argument("AnnoyConfig.root_path must not be empty.");
    }
    std::error_code error;
    std::filesystem::remove_all(config.root_path, error);
    if (error) throw std::runtime_error("Failed to remove Annoy directory: " + error.message());
}

void AnnoyBackend::open(const config_type &config)
{
    if (!impl_) impl_ = std::make_unique<Impl>();
    if (impl_->is_open) throw std::logic_error("Annoy backend is already open.");
    validate_config(config);
    if (config.open_options.truncate_if_exists) destroy(config);
    if (!std::filesystem::exists(config.root_path)) {
        if (!config.open_options.create_if_missing) {
            throw std::out_of_range("Annoy backend directory does not exist.");
        }
        create(config);
    }
    impl_->config = config;
    impl_->is_open = true;
    try { impl_->load(); } catch (...) { impl_->reset(); throw; }
}

void AnnoyBackend::close()
{
    if (!impl_ || !impl_->is_open) return;
    if (!impl_->config.open_options.read_only) impl_->persist();
    impl_->reset();
}

bool AnnoyBackend::contains(std::string_view key) const
{
    impl_->require_open();
    return impl_->records.contains(std::string(key));
}

nearest::Item AnnoyBackend::get_item(std::string_view key) const
{
    impl_->require_open();
    const auto found = impl_->records.find(std::string(key));
    if (found == impl_->records.end()) throw std::out_of_range("Annoy item was not found.");
    return found->second;
}

void AnnoyBackend::upsert_item(const nearest::Item &item)
{
    impl_->require_writable();
    if (item.id.empty()) throw std::invalid_argument("Annoy item id must not be empty.");
    if (item.embedding.size() != impl_->config.dimension) {
        throw std::invalid_argument("Annoy item dimension does not match configuration.");
    }
    impl_->records[item.id] = item;
    impl_->dirty = true;
}

void AnnoyBackend::erase_item(std::string_view key)
{
    impl_->require_writable();
    impl_->records.erase(std::string(key));
    impl_->dirty = true;
}

std::vector<nearest::SearchHit> AnnoyBackend::search(
    std::span<const float> query, nearest::SearchOptions options) const
{
    impl_->require_open();
    if (query.size() != impl_->config.dimension) {
        throw std::invalid_argument("Annoy query dimension does not match configuration.");
    }
    if (options.limit == 0 || impl_->records.empty()) return {};
    impl_->rebuild();
    std::vector<int> ids;
    std::vector<float> distances;
    impl_->index->get_nns_by_vector(
        query.data(), options.limit, options.search_k, &ids, &distances);
    std::vector<nearest::SearchHit> output;
    for (std::size_t index = 0; index < ids.size(); ++index) {
        const auto item = impl_->records.find(impl_->ids.at(ids[index]));
        if (item == impl_->records.end()) continue;
        output.push_back({ item->second.id,
                           index < distances.size() ? distances[index] : 0.0F,
                           item->second.text_payload,
                           item->second.bytes_payload });
    }
    return output;
}

std::vector<Key> AnnoyBackend::list_keys() const
{
    impl_->require_open();
    return impl_->sorted_keys();
}

} // namespace PDJE_UTIL::db::backends
