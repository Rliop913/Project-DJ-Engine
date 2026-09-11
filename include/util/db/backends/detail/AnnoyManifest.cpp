#include "AnnoyManifest.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <system_error>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace PDJE_UTIL::db::backends::detail {
namespace {

std::string
encode(std::span<const std::byte> input)
{
    if (input.size() > std::numeric_limits<std::size_t>::max() / 2u) {
        throw std::overflow_error("Annoy manifest value is too large.");
    }
    static constexpr char digits[] = "0123456789abcdef";
    std::string           output(input.size() * 2u, '0');
    for (std::size_t index = 0; index < input.size(); ++index) {
        const auto value        = std::to_integer<unsigned int>(input[index]);
        output[index * 2u]      = digits[value >> 4u];
        output[index * 2u + 1u] = digits[value & 0x0fu];
    }
    return output;
}

std::string
encode(std::string_view input)
{
    return encode(
        { reinterpret_cast<const std::byte *>(input.data()), input.size() });
}

int
nibble(char value)
{
    if (value >= '0' && value <= '9')
        return value - '0';
    if (value >= 'a' && value <= 'f')
        return value - 'a' + 10;
    if (value >= 'A' && value <= 'F')
        return value - 'A' + 10;
    return -1;
}

std::vector<std::byte>
decode(std::string_view input)
{
    if ((input.size() % 2u) != 0u) {
        throw std::runtime_error(
            "Annoy manifest contains invalid hexadecimal data.");
    }
    std::vector<std::byte> output;
    output.reserve(input.size() / 2u);
    for (std::size_t index = 0; index < input.size(); index += 2u) {
        const int high = nibble(input[index]);
        const int low  = nibble(input[index + 1u]);
        if (high < 0 || low < 0) {
            throw std::runtime_error(
                "Annoy manifest contains invalid hexadecimal data.");
        }
        output.push_back(static_cast<std::byte>((high << 4) | low));
    }
    return output;
}

std::string
decode_text(std::string_view input)
{
    const auto bytes = decode(input);
    return { reinterpret_cast<const char *>(bytes.data()), bytes.size() };
}

nearest::Embedding
decode_embedding(std::string_view input, std::size_t dimension)
{
    if (dimension > std::numeric_limits<std::size_t>::max() / sizeof(float)) {
        throw std::runtime_error("Annoy manifest dimension is too large.");
    }
    const auto bytes = decode(input);
    if (bytes.size() != dimension * sizeof(float)) {
        throw std::runtime_error(
            "Annoy manifest embedding dimension is invalid.");
    }
    nearest::Embedding output(dimension);
    if (!bytes.empty())
        std::memcpy(output.data(), bytes.data(), bytes.size());
    return output;
}

std::vector<std::string>
split_fields(const std::string &line)
{
    std::vector<std::string> fields;
    std::istringstream       stream(line);
    for (std::string field; std::getline(stream, field, '\t');)
        fields.push_back(std::move(field));
    if (!line.empty() && line.back() == '\t')
        fields.emplace_back();
    return fields;
}

class TemporaryFile {
  public:
    explicit TemporaryFile(std::filesystem::path path) : path_(std::move(path))
    {
    }
    ~TemporaryFile()
    {
        if (!active_)
            return;
        std::error_code ignored;
        std::filesystem::remove(path_, ignored);
    }
    void
    release() noexcept
    {
        active_ = false;
    }

  private:
    std::filesystem::path path_;
    bool                  active_ = true;
};

void
replace_file(const std::filesystem::path &temporary,
             const std::filesystem::path &target)
{
#if defined(_WIN32)
    if (::MoveFileExW(temporary.c_str(),
                      target.c_str(),
                      MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) ==
        0) {
        throw std::system_error(static_cast<int>(::GetLastError()),
                                std::system_category(),
                                "Failed to replace Annoy manifest");
    }
#else
    std::error_code error;
    std::filesystem::rename(temporary, target, error);
    if (error) {
        throw std::system_error(error, "Failed to replace Annoy manifest");
    }
#endif
}

} // namespace

AnnoyRecords
load_annoy_manifest(const AnnoyConfig &config)
{
    AnnoyRecords records;
    const auto   path = config.root_path / "records.tsv";
    if (!std::filesystem::exists(path))
        return records;

    std::ifstream input(path);
    if (!input)
        throw std::runtime_error("Failed to open Annoy manifest file.");
    for (std::string line; std::getline(input, line);) {
        if (line.empty())
            continue;
        const auto fields = split_fields(line);
        if (fields.size() != 6u)
            throw std::runtime_error("Annoy manifest is malformed.");

        nearest::Item item{ .id = decode_text(fields[0]),
                            .embedding =
                                decode_embedding(fields[1], config.dimension) };
        if (fields[2] == "1")
            item.text_payload = decode_text(fields[3]);
        else if (fields[2] != "0")
            throw std::runtime_error("Invalid Annoy text flag.");
        if (fields[4] == "1")
            item.bytes_payload = decode(fields[5]);
        else if (fields[4] != "0")
            throw std::runtime_error("Invalid Annoy bytes flag.");
        if (!records.emplace(item.id, std::move(item)).second)
            throw std::runtime_error("Annoy manifest contains duplicate ids.");
    }
    if (!input.eof())
        throw std::runtime_error("Failed to read Annoy manifest file.");
    return records;
}

void
save_annoy_manifest_atomic(const AnnoyConfig  &config,
                           const AnnoyRecords &records)
{
    const auto    target    = config.root_path / "records.tsv";
    const auto    temporary = config.root_path / "records.tsv.tmp";
    TemporaryFile cleanup(temporary);

    std::vector<Key> keys;
    keys.reserve(records.size());
    for (const auto &[key, item] : records)
        keys.push_back(key);
    std::sort(keys.begin(), keys.end());

    std::ofstream output(temporary, std::ios::trunc);
    if (!output)
        throw std::runtime_error("Failed to write Annoy manifest file.");
    for (const auto &key : keys) {
        const auto &item      = records.at(key);
        const auto  embedding = std::span(
            reinterpret_cast<const std::byte *>(item.embedding.data()),
            item.embedding.size() * sizeof(float));
        output << encode(item.id) << '\t' << encode(embedding) << '\t'
               << (item.text_payload ? "1\t" + encode(*item.text_payload)
                                     : "0\t")
               << '\t'
               << (item.bytes_payload ? "1\t" + encode(*item.bytes_payload)
                                      : "0\t")
               << '\n';
        if (!output)
            throw std::runtime_error("Failed to write Annoy manifest file.");
    }
    output.flush();
    if (!output)
        throw std::runtime_error("Failed to flush Annoy manifest file.");
    output.close();
    if (!output)
        throw std::runtime_error("Failed to close Annoy manifest file.");

    replace_file(temporary, target);
    cleanup.release();
}

} // namespace PDJE_UTIL::db::backends::detail
