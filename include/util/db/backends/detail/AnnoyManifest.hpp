#pragma once

#include "util/db/backends/AnnoyBackend.hpp"

#include <unordered_map>

namespace PDJE_UTIL::db::backends::detail {

using AnnoyRecords = std::unordered_map<Key, nearest::Item>;

AnnoyRecords
load_annoy_manifest(const AnnoyConfig &config);

void
save_annoy_manifest_atomic(const AnnoyConfig  &config,
                           const AnnoyRecords &records);

} // namespace PDJE_UTIL::db::backends::detail
