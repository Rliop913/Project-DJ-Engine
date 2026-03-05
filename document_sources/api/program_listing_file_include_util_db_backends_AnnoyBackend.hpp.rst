
.. _program_listing_file_include_util_db_backends_AnnoyBackend.hpp:

Program Listing for File AnnoyBackend.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_backends_AnnoyBackend.hpp>` (``include\util\db\backends\AnnoyBackend.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/Result.hpp"
   #include "util/db/DbTypes.hpp"
   #include "util/db/nearest/Types.hpp"
   
   #include <annoy/annoylib.h>
   #include <annoy/kissrandom.h>
   
   #include <algorithm>
   #include <cstddef>
   #include <cstdlib>
   #include <cstring>
   #include <filesystem>
   #include <fstream>
   #include <map>
   #include <memory>
   #include <optional>
   #include <span>
   #include <sstream>
   #include <string>
   #include <string_view>
   #include <unordered_map>
   #include <utility>
   #include <vector>
   
   namespace PDJE_UTIL::db::backends {
   
   namespace detail {
   
   inline std::string
   hex_encode(std::string_view input)
   {
       static constexpr char hex_digits[] = "0123456789abcdef";
       std::string           encoded;
       encoded.reserve(input.size() * 2);
       for (unsigned char ch : input) {
           encoded.push_back(hex_digits[(ch >> 4) & 0x0F]);
           encoded.push_back(hex_digits[ch & 0x0F]);
       }
       return encoded;
   }
   
   inline std::string
   hex_encode_bytes(std::span<const std::byte> input)
   {
       if (input.empty()) {
           return {};
       }
       return hex_encode(std::string_view(reinterpret_cast<const char *>(input.data()),
                                          input.size_bytes()));
   }
   
   inline std::optional<std::vector<std::byte>>
   hex_decode_bytes(std::string_view input)
   {
       if ((input.size() % 2) != 0) {
           return std::nullopt;
       }
   
       auto decode_nibble = [](unsigned char ch) -> int {
           if (ch >= '0' && ch <= '9') {
               return ch - '0';
           }
           if (ch >= 'a' && ch <= 'f') {
               return 10 + ch - 'a';
           }
           if (ch >= 'A' && ch <= 'F') {
               return 10 + ch - 'A';
           }
           return -1;
       };
   
       std::vector<std::byte> bytes;
       bytes.reserve(input.size() / 2);
       for (std::size_t i = 0; i < input.size(); i += 2) {
           const int hi = decode_nibble(static_cast<unsigned char>(input[i]));
           const int lo = decode_nibble(static_cast<unsigned char>(input[i + 1]));
           if (hi < 0 || lo < 0) {
               return std::nullopt;
           }
           bytes.push_back(static_cast<std::byte>((hi << 4) | lo));
       }
       return bytes;
   }
   
   inline std::optional<std::string>
   hex_decode_text(std::string_view input)
   {
       auto bytes = hex_decode_bytes(input);
       if (!bytes.has_value()) {
           return std::nullopt;
       }
       if (bytes->empty()) {
           return std::string {};
       }
       return std::string(reinterpret_cast<const char *>(bytes->data()), bytes->size());
   }
   
   inline std::string
   encode_embedding(std::span<const float> embedding)
   {
       if (embedding.empty()) {
           return {};
       }
       const auto *bytes = reinterpret_cast<const std::byte *>(embedding.data());
       return hex_encode_bytes(std::span<const std::byte>(
           bytes, embedding.size() * sizeof(float)));
   }
   
   inline std::optional<nearest::Embedding>
   decode_embedding(std::string_view input, std::size_t expected_dimension)
   {
       auto bytes = hex_decode_bytes(input);
       if (!bytes.has_value()) {
           return std::nullopt;
       }
       if ((bytes->size() % sizeof(float)) != 0) {
           return std::nullopt;
       }
   
       nearest::Embedding embedding(bytes->size() / sizeof(float));
       if (!bytes->empty()) {
           std::memcpy(embedding.data(), bytes->data(), bytes->size());
       }
       if (embedding.size() != expected_dimension) {
           return std::nullopt;
       }
       return embedding;
   }
   
   inline std::filesystem::path
   annoy_manifest_path(const std::filesystem::path &root_path)
   {
       return root_path / "records.tsv";
   }
   
   inline std::filesystem::path
   annoy_index_path(const std::filesystem::path &root_path)
   {
       return root_path / "index.ann";
   }
   
   inline void
   free_annoy_error(char *error)
   {
       if (error != nullptr) {
           std::free(error);
       }
   }
   
   inline std::vector<std::string>
   split_tsv_line(const std::string &line)
   {
       std::vector<std::string> fields;
       std::istringstream       stream(line);
       std::string              field;
       while (std::getline(stream, field, '\t')) {
           fields.push_back(field);
       }
       if (!line.empty() && line.back() == '\t') {
           fields.emplace_back();
       }
       return fields;
   }
   
   } // namespace detail
   
   struct AnnoyConfig {
       std::filesystem::path root_path;
       OpenOptions           open_options {};
       std::size_t           dimension = 0;
       int                   trees     = 10;
       bool                  prefault  = false;
   };
   
   class AnnoyBackend {
     public:
       using config_type = AnnoyConfig;
       using IndexType =
           Annoy::AnnoyIndex<int,
                             float,
                             Annoy::Angular,
                             Annoy::Kiss32Random,
                             Annoy::AnnoyIndexSingleThreadedBuildPolicy>;
   
       static common::Result<void>
       create(const config_type &cfg)
       {
           if (cfg.root_path.empty()) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "AnnoyConfig.root_path must not be empty." });
           }
           if (cfg.dimension == 0) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "AnnoyConfig.dimension must be greater than zero." });
           }
   
           std::error_code ec;
           std::filesystem::create_directories(cfg.root_path, ec);
           if (ec) {
               return common::Result<void>::failure(
                   { common::StatusCode::io_error, ec.message() });
           }
           return common::Result<void>::success();
       }
   
       static common::Result<void>
       destroy(const config_type &cfg)
       {
           if (cfg.root_path.empty()) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "AnnoyConfig.root_path must not be empty." });
           }
   
           std::error_code ec;
           std::filesystem::remove_all(cfg.root_path, ec);
           if (ec) {
               return common::Result<void>::failure(
                   { common::StatusCode::io_error, ec.message() });
           }
           return common::Result<void>::success();
       }
   
       common::Result<void>
       open(const config_type &cfg)
       {
           if (is_open_) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument, "Annoy backend is already open." });
           }
           if (cfg.root_path.empty()) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "AnnoyConfig.root_path must not be empty." });
           }
           if (cfg.dimension == 0) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "AnnoyConfig.dimension must be greater than zero." });
           }
           if (cfg.open_options.read_only &&
               (cfg.open_options.create_if_missing || cfg.open_options.truncate_if_exists)) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "Annoy read-only mode cannot create or truncate the index." });
           }
   
           config_ = cfg;
           if (config_.open_options.truncate_if_exists) {
               auto destroyed = destroy(config_);
               if (!destroyed.ok()) {
                   return destroyed;
               }
           }
   
           const bool exists = std::filesystem::exists(config_.root_path);
           if (!exists) {
               if (config_.open_options.create_if_missing) {
                   auto created = create(config_);
                   if (!created.ok()) {
                       return created;
                   }
               } else {
                   return common::Result<void>::failure(
                       { common::StatusCode::not_found,
                         "Annoy backend directory does not exist." });
               }
           }
   
           records_.clear();
           id_to_key_.clear();
           index_.reset();
           next_item_id_ = 0;
           index_dirty_  = true;
           is_open_      = true;
   
           const auto manifest_path = detail::annoy_manifest_path(config_.root_path);
           if (std::filesystem::exists(manifest_path)) {
               auto loaded = load_manifest(manifest_path);
               if (!loaded.ok()) {
                   reset_runtime_state();
                   return loaded;
               }
           }
   
           return common::Result<void>::success();
       }
   
       common::Result<void>
       close()
       {
           if (!is_open_) {
               return common::Result<void>::success();
           }
   
           common::Result<void> result = common::Result<void>::success();
           if (!config_.open_options.read_only) {
               result = persist();
           }
   
           reset_runtime_state();
           return result;
       }
   
       common::Result<bool>
       contains(std::string_view key) const
       {
           if (auto status = require_open(); !status.ok()) {
               return common::Result<bool>::failure(status);
           }
           return common::Result<bool>::success(records_.contains(std::string(key)));
       }
   
       common::Result<nearest::Item>
       get_item(std::string_view key) const
       {
           if (auto status = require_open(); !status.ok()) {
               return common::Result<nearest::Item>::failure(status);
           }
   
           const auto found = records_.find(std::string(key));
           if (found == records_.end()) {
               return common::Result<nearest::Item>::failure(
                   { common::StatusCode::not_found, "Annoy item was not found." });
           }
           return common::Result<nearest::Item>::success(found->second);
       }
   
       common::Result<void>
       upsert_item(const nearest::Item &item)
       {
           if (auto status = require_writable(); !status.ok()) {
               return common::Result<void>::failure(status);
           }
           if (item.id.empty()) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument, "Annoy item id must not be empty." });
           }
           if (item.embedding.size() != config_.dimension) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "Annoy item embedding size must match the configured dimension." });
           }
   
           records_[item.id] = item;
           index_dirty_      = true;
           return common::Result<void>::success();
       }
   
       common::Result<void>
       erase_item(std::string_view key)
       {
           if (auto status = require_writable(); !status.ok()) {
               return common::Result<void>::failure(status);
           }
   
           records_.erase(std::string(key));
           index_dirty_ = true;
           return common::Result<void>::success();
       }
   
       common::Result<std::vector<nearest::SearchHit>>
       search(std::span<const float> query, nearest::SearchOptions options) const
       {
           if (auto status = require_open(); !status.ok()) {
               return common::Result<std::vector<nearest::SearchHit>>::failure(status);
           }
           if (query.size() != config_.dimension) {
               return common::Result<std::vector<nearest::SearchHit>>::failure(
                   { common::StatusCode::invalid_argument,
                     "Annoy query vector size must match the configured dimension." });
           }
           if (options.limit == 0 || records_.empty()) {
               return common::Result<std::vector<nearest::SearchHit>>::success({});
           }
   
           auto rebuilt = rebuild_index();
           if (!rebuilt.ok()) {
               return common::Result<std::vector<nearest::SearchHit>>::failure(rebuilt.status());
           }
           if (!index_) {
               return common::Result<std::vector<nearest::SearchHit>>::success({});
           }
   
           std::vector<int>   ids;
           std::vector<float> distances;
           index_->get_nns_by_vector(query.data(), options.limit, options.search_k, &ids, &distances);
   
           std::vector<nearest::SearchHit> hits;
           hits.reserve(ids.size());
           for (std::size_t i = 0; i < ids.size(); ++i) {
               const auto found_key = id_to_key_.find(ids[i]);
               if (found_key == id_to_key_.end()) {
                   continue;
               }
   
               const auto found_item = records_.find(found_key->second);
               if (found_item == records_.end()) {
                   continue;
               }
   
               hits.push_back(
                   { found_item->second.id,
                     i < distances.size() ? distances[i] : 0.0F,
                     found_item->second.text_payload,
                     found_item->second.bytes_payload });
           }
   
           return common::Result<std::vector<nearest::SearchHit>>::success(std::move(hits));
       }
   
       common::Result<std::vector<Key>>
       list_keys() const
       {
           if (auto status = require_open(); !status.ok()) {
               return common::Result<std::vector<Key>>::failure(status);
           }
   
           std::vector<Key> keys;
           keys.reserve(records_.size());
           for (const auto &[key, _] : records_) {
               keys.push_back(key);
           }
           std::sort(keys.begin(), keys.end());
           return common::Result<std::vector<Key>>::success(std::move(keys));
       }
   
     private:
       common::Status
       require_open() const
       {
           if (!is_open_) {
               return { common::StatusCode::closed, "Annoy backend is not open." };
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
               return { common::StatusCode::unsupported, "Annoy backend is opened read-only." };
           }
           return {};
       }
   
       common::Result<void>
       rebuild_index() const
       {
           if (!is_open_) {
               return common::Result<void>::failure(
                   { common::StatusCode::closed, "Annoy backend is not open." });
           }
           if (!index_dirty_) {
               return common::Result<void>::success();
           }
   
           index_ = std::make_unique<IndexType>(static_cast<int>(config_.dimension));
           id_to_key_.clear();
           next_item_id_ = 0;
   
           for (const auto &[key, item] : records_) {
               if (item.embedding.size() != config_.dimension) {
                   return common::Result<void>::failure(
                       { common::StatusCode::backend_error,
                         "Annoy manifest contains an embedding with the wrong dimension." });
               }
   
               const int item_id = next_item_id_++;
               char     *error   = nullptr;
               if (!index_->add_item(item_id, item.embedding.data(), &error)) {
                   std::string message = error != nullptr ? error : "Annoy add_item failed.";
                   detail::free_annoy_error(error);
                   return common::Result<void>::failure(
                       { common::StatusCode::backend_error, std::move(message) });
               }
               id_to_key_[item_id] = key;
           }
   
           if (!records_.empty()) {
               char *error = nullptr;
               if (!index_->build(config_.trees, -1, &error)) {
                   std::string message = error != nullptr ? error : "Annoy build failed.";
                   detail::free_annoy_error(error);
                   return common::Result<void>::failure(
                       { common::StatusCode::backend_error, std::move(message) });
               }
           }
   
           index_dirty_ = false;
           return common::Result<void>::success();
       }
   
       common::Result<void>
       load_manifest(const std::filesystem::path &manifest_path)
       {
           std::ifstream input(manifest_path);
           if (!input) {
               return common::Result<void>::failure(
                   { common::StatusCode::io_error, "Failed to open Annoy manifest file." });
           }
   
           std::string line;
           while (std::getline(input, line)) {
               if (line.empty()) {
                   continue;
               }
   
               const auto fields = detail::split_tsv_line(line);
               if (fields.size() != 6) {
                   return common::Result<void>::failure(
                       { common::StatusCode::backend_error, "Annoy manifest is malformed." });
               }
   
               auto id = detail::hex_decode_text(fields[0]);
               auto embedding = detail::decode_embedding(fields[1], config_.dimension);
               if (!id.has_value() || !embedding.has_value()) {
                   return common::Result<void>::failure(
                       { common::StatusCode::backend_error,
                         "Annoy manifest contains malformed item data." });
               }
   
               nearest::Item item;
               item.id        = std::move(*id);
               item.embedding = std::move(*embedding);
   
               if (fields[2] == "1") {
                   auto text_payload = detail::hex_decode_text(fields[3]);
                   if (!text_payload.has_value()) {
                       return common::Result<void>::failure(
                           { common::StatusCode::backend_error,
                             "Annoy manifest text payload is malformed." });
                   }
                   item.text_payload = std::move(*text_payload);
               } else if (fields[2] != "0") {
                   return common::Result<void>::failure(
                       { common::StatusCode::backend_error,
                         "Annoy manifest text payload flag is malformed." });
               }
   
               if (fields[4] == "1") {
                   auto bytes_payload = detail::hex_decode_bytes(fields[5]);
                   if (!bytes_payload.has_value()) {
                       return common::Result<void>::failure(
                           { common::StatusCode::backend_error,
                             "Annoy manifest bytes payload is malformed." });
                   }
                   item.bytes_payload = std::move(*bytes_payload);
               } else if (fields[4] != "0") {
                   return common::Result<void>::failure(
                       { common::StatusCode::backend_error,
                         "Annoy manifest bytes payload flag is malformed." });
               }
   
               records_[item.id] = std::move(item);
           }
   
           return common::Result<void>::success();
       }
   
       common::Result<void>
       persist()
       {
           auto rebuilt = rebuild_index();
           if (!rebuilt.ok()) {
               return rebuilt;
           }
   
           std::error_code ec;
           std::filesystem::create_directories(config_.root_path, ec);
           if (ec) {
               return common::Result<void>::failure(
                   { common::StatusCode::io_error, ec.message() });
           }
   
           const auto manifest_path = detail::annoy_manifest_path(config_.root_path);
           std::ofstream output(manifest_path, std::ios::trunc);
           if (!output) {
               return common::Result<void>::failure(
                   { common::StatusCode::io_error, "Failed to write Annoy manifest file." });
           }
   
           auto keys_result = list_keys();
           if (!keys_result.ok()) {
               return common::Result<void>::failure(keys_result.status());
           }
   
           for (const auto &key : keys_result.value()) {
               const auto &item = records_.at(key);
               output << detail::hex_encode(item.id) << '\t'
                      << detail::encode_embedding(item.embedding) << '\t'
                      << (item.text_payload.has_value() ? "1" : "0") << '\t'
                      << (item.text_payload.has_value() ? detail::hex_encode(*item.text_payload)
                                                        : std::string {})
                      << '\t'
                      << (item.bytes_payload.has_value() ? "1" : "0") << '\t'
                      << (item.bytes_payload.has_value()
                              ? detail::hex_encode_bytes(*item.bytes_payload)
                              : std::string {})
                      << '\n';
           }
           output.close();
   
           const auto index_path = detail::annoy_index_path(config_.root_path);
           if (records_.empty()) {
               std::filesystem::remove(index_path, ec);
               return common::Result<void>::success();
           }
   
           char *error = nullptr;
           if (!index_->save(index_path.string().c_str(), config_.prefault, &error)) {
               std::string message = error != nullptr ? error : "Annoy save failed.";
               detail::free_annoy_error(error);
               return common::Result<void>::failure(
                   { common::StatusCode::backend_error, std::move(message) });
           }
   
           return common::Result<void>::success();
       }
   
       void
       reset_runtime_state()
       {
           index_.reset();
           id_to_key_.clear();
           records_.clear();
           next_item_id_ = 0;
           index_dirty_  = true;
           is_open_      = false;
       }
   
       config_type                                    config_ {};
       bool                                           is_open_ = false;
       mutable bool                                   index_dirty_ = true;
       mutable int                                    next_item_id_ = 0;
       std::unordered_map<Key, nearest::Item>         records_ {};
       mutable std::map<int, Key>                     id_to_key_ {};
       mutable std::unique_ptr<IndexType>             index_ {};
   };
   
   } // namespace PDJE_UTIL::db::backends
