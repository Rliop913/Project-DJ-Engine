Util_Engine
===========

The Utility layer is active code in the current tree. It is centered around the
umbrella header `util/PDJE_Util.hpp` and a set of header-first abstractions that
can be linked through the `PDJE_UTIL` interface targets declared in
`cmakes/src/UTILsrc.cmake`.

Overview
--------

`PDJE_UTIL` currently groups four areas:

- `PDJE_UTIL::common` for error/status transport.
- `PDJE_UTIL::db` for generic key-value, relational, and nearest-neighbor
  storage abstractions.
- `PDJE_UTIL::function` for lightweight helper functions and evaluation context
  types.
- `PDJE_UTIL::ai` as a placeholder namespace for future higher-level AI helpers.

Umbrella Header
---------------

.. code-block:: c++

   #include "util/PDJE_Util.hpp"

This header re-exports the currently maintained common/status types, database
wrappers, backend adapters, and function helpers.

Common Status Types
-------------------

.. doxygenfile:: include/util/common/StatusCode.hpp
   :project: Project_DJ_Engine

.. doxygenfile:: include/util/common/Status.hpp
   :project: Project_DJ_Engine

.. doxygenfile:: include/util/common/Result.hpp
   :project: Project_DJ_Engine

`StatusCode` defines a compact error taxonomy such as `invalid_argument`,
`not_found`, `io_error`, `backend_error`, and `internal_error`.

`Status` stores the code plus a human-readable message.

`Result<T>` and `Result<void>` wrap either a value or a failure `Status`,
providing a single convention used by the utility APIs.

Example:

.. code-block:: c++

   using namespace PDJE_UTIL;

   auto value = function::clamp({ .value = 1.4, .min_value = 0.0, .max_value = 1.0 });
   if (!value.ok()) {
       std::cerr << value.status().message << std::endl;
       return;
   }

   std::cout << value.value() << std::endl;

Database Abstractions
---------------------

The database layer is split by access pattern:

- `db::Database<Backend>` is the convenience alias for key-value backends.
- `db::keyvalue::KeyValueDatabase<Backend>` is the generic key-value wrapper.
- `db::relational::RelationalDatabase<Backend>` is the SQL/relational wrapper.
- `db::nearest::NearestNeighborIndex<Backend>` is the embedding search wrapper.

Each wrapper follows the same lifecycle shape:

- `create(config)`
- `destroy(config)`
- `open(config)`
- `close()`

They return `common::Result<...>` values rather than throwing utility-specific
exceptions.

Backends shipped in the current tree:

- `db::backends::SqliteBackend`
- `db::backends::RocksDbBackend`
- `db::backends::AnnoyBackend`

The CMake interface targets expose the corresponding dependencies:

- `PDJE_UTIL_DB_SQLITE`
- `PDJE_UTIL_DB_ROCKSDB`
- `PDJE_UTIL_DB_ANNOY`
- `PDJE_UTIL_DB`

Key-Value Example
~~~~~~~~~~~~~~~~~

.. code-block:: c++

   using namespace PDJE_UTIL;

   db::backends::RocksDbConfig cfg;
   cfg.path = "tmp/example-rocks";
   cfg.open_options.create_if_missing = true;

   auto opened = db::keyvalue::KeyValueDatabase<db::backends::RocksDbBackend>::open(cfg);
   if (!opened.ok()) {
       return;
   }

   auto db = std::move(opened.value());
   (void)db.put_text("artist", "RLIOP913");
   (void)db.put_bytes("blob", {});

Relational Example
~~~~~~~~~~~~~~~~~~

.. code-block:: c++

   using namespace PDJE_UTIL;

   db::backends::SqliteConfig cfg;
   cfg.path = "tmp/example.sqlite3";
   cfg.open_options.create_if_missing = true;

   auto opened = db::relational::RelationalDatabase<db::backends::SqliteBackend>::open(cfg);
   if (!opened.ok()) {
       return;
   }

   auto db = std::move(opened.value());
   (void)db.execute("CREATE TABLE IF NOT EXISTS notes(id INTEGER PRIMARY KEY, lane TEXT)");

Nearest-Neighbor Example
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: c++

   using namespace PDJE_UTIL;

   db::backends::AnnoyConfig cfg;
   cfg.root_path = "tmp/example-annoy";
   cfg.dimension = 4;
   cfg.open_options.create_if_missing = true;

   auto opened =
       db::nearest::NearestNeighborIndex<db::backends::AnnoyBackend>::open(cfg);
   if (!opened.ok()) {
       return;
   }

   auto index = std::move(opened.value());
   db::nearest::Item item;
   item.id = "kick";
   item.embedding = {1.0f, 0.0f, 0.0f, 0.0f};
   (void)index.upsert_item(item);

Function Helpers
----------------

`PDJE_UTIL::function` currently contains small, composable helpers.

- `clamp(ClampArgs)` clamps a floating-point value inside a range.
- `slugify(SlugifyArgs, EvalOptions)` normalizes text into a delimiter-based
  slug.
- `halide::apply_gain_bias(GrayImageView, GainBiasArgs, EvalOptions)` applies a
  simple grayscale gain/bias transform and returns a new `GrayImage`.

Example:

.. code-block:: c++

   using namespace PDJE_UTIL;

   auto slug = function::slugify({ .input = "Project DJ Engine", .separator = '-' });
   if (slug.ok()) {
       std::cout << slug.value() << std::endl; // "project-dj-engine"
   }

AI Namespace
------------

`PDJE_UTIL::ai` currently exists as a placeholder namespace only. There are no
public AI utility functions defined in the current header set.

Generated API
-------------

Use :doc:`/api/api_root` if you need the file-by-file generated view of utility
headers and backend classes.
