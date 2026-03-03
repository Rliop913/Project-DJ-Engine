#pragma once

#include "util/db/keyvalue/BackendConcept.hpp"

namespace PDJE_UTIL::db {

template <class Backend>
concept BackendConcept = keyvalue::KeyValueBackendConcept<Backend>;

} // namespace PDJE_UTIL::db
