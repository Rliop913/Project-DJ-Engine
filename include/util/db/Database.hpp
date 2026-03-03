#pragma once

#include "util/db/keyvalue/Database.hpp"

namespace PDJE_UTIL::db {

template <keyvalue::KeyValueBackendConcept Backend>
using Database = keyvalue::KeyValueDatabase<Backend>;

} // namespace PDJE_UTIL::db
