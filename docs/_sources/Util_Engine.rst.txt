Util_Engine
===========

This page documents the current utility surface that is backed by the source
tree in this repository. For this module, the hand-written source of truth is:

- `include/util/`
- `tests/unit/util/`
- `cmakes/src/UTIL/UTILsrc.cmake`

Generated API pages under :doc:`/api/api_root` remain useful for symbol lookup,
but this guide is the recommended starting point for the maintained utility
surface and the intended consumption boundaries.

Public Consumption Units
------------------------

The current CMake targets split the utility layer into four consumer-facing
units:

- `PDJE_UTIL`
  base interface target for the utility include tree and the umbrella header
  path
- `PDJE_UTIL_DB`
  aggregate database target that wires the SQLite, RocksDB, and Annoy utility
  backends
- `PDJE_UTIL_IMAGE_WEBP`
  image/WebP helper target for the generic WebP writer path
- `PDJE_UTIL_IMAGE_WAVEFORM`
  waveform image generation target that also compiles the STFT and backend
  loader implementation files

In other words, the utility layer is not roadmap-only. It is active code with
headers, tests, and CMake consumption points in the current tree.

Header Boundaries
-----------------

The utility surface is intentionally split between one umbrella header and a
few direct-include headers.

Umbrella header:

.. code-block:: c++

   #include "util/PDJE_Util.hpp"

This re-exports the currently maintained:

- `PDJE_UTIL::common::{StatusCode, Status, Result<T>, Result<void>}`
- database wrapper types and backend concepts
- `db::Database<Backend>` as the key-value convenience alias
- `PDJE_UTIL::function::{clamp, slugify, EvalOptions, CacheContext}`

Direct include headers still required for APIs that are not re-exported by the
umbrella header:

- `util/function/image/WebpWriter.hpp`
- `util/function/image/WaveformWebp.hpp`
- `util/function/stft/STFT_Parallel.hpp`
- `util/common/BackendLoader/OpenCL_Loader.hpp`
- `util/common/BackendLoader/PDJE_Parallel_Runtime_Loader.hpp`

Common Status And Result Types
------------------------------

The utility module uses one error transport pattern everywhere: `StatusCode`,
`Status`, and `Result<T>`.

.. doxygenfile:: include/util/common/StatusCode.hpp
   :project: Project_DJ_Engine

.. doxygenfile:: include/util/common/Status.hpp
   :project: Project_DJ_Engine

.. doxygenfile:: include/util/common/Result.hpp
   :project: Project_DJ_Engine

Current facts from the headers and tests:

- `StatusCode` is the shared error taxonomy for utility APIs
- `Status` stores `code` plus a human-readable `message`
- `Status::ok()` is true only when `code == StatusCode::ok`
- `Result<T>` stores either a value plus ok status, or a failure status
- `Result<void>` keeps the same success/failure contract without a value

The header set and util status tests currently define and reference codes such
as:

- `invalid_argument`
- `not_found`
- `type_mismatch`
- `unsupported`
- `io_error`
- `closed`
- `backend_error`
- `out_of_range`
- `internal_error`

Typical usage looks like this:

.. code-block:: c++

   #include "util/PDJE_Util.hpp"

   using namespace PDJE_UTIL;

   auto clamped = function::clamp(
       { .value = 2.5, .min_value = 0.0, .max_value = 1.0 });
   if (!clamped.ok()) {
       std::cerr << clamped.status().message << std::endl;
       return;
   }

   std::cout << clamped.value() << std::endl;

Database Wrappers
-----------------

The current utility DB layer is organized by access pattern, not by one global
database class.

Wrapper split:

.. list-table::
   :header-rows: 1
   :widths: 28 32 40

   * - Wrapper
     - Typical backend
     - Current role
   * - `db::Database<Backend>`
     - key-value backend
     - convenience alias for `db::keyvalue::KeyValueDatabase<Backend>`
   * - `db::keyvalue::KeyValueDatabase<Backend>`
     - `db::backends::RocksDbBackend`
     - text/blob lookup, overwrite, erase, prefix scan
   * - `db::relational::RelationalDatabase<Backend>`
     - `db::backends::SqliteBackend`
     - SQL execution, queries, transactions
   * - `db::nearest::NearestNeighborIndex<Backend>`
     - `db::backends::AnnoyBackend`
     - embedding storage and nearest-neighbor search

All three wrapper families follow the same high-level lifecycle:

- `create(config)`
- `open(config)`
- use the opened object
- `close()`
- optionally `destroy(config)`

The shipped tests validate concrete behavior rather than only type presence:

- RocksDB key-value:
  text writes, byte writes, overwrite, prefix `list_keys()`, erase, read-only
  rejection, and type mismatch reporting
- SQLite relational:
  execute/query, parameter binding, blob transport, transaction rollback and
  commit, and read-only rejection
- Annoy nearest-neighbor:
  upsert, get, search, erase, reopen/persistence, read-only rejection, and
  invalid embedding dimension handling

RocksDB key-value example:

.. code-block:: c++

   #include "util/db/backends/RocksDbBackend.hpp"
   #include "util/db/keyvalue/Database.hpp"

   using Db = PDJE_UTIL::db::keyvalue::KeyValueDatabase<
       PDJE_UTIL::db::backends::RocksDbBackend>;

   PDJE_UTIL::db::backends::RocksDbConfig cfg{
       .path = "tmp/example-rocks",
       .open_options = { .create_if_missing = true }
   };

   auto opened = Db::open(cfg);
   if (!opened.ok()) {
       return;
   }

   auto db = std::move(opened.value());
   (void)db.put_text("artist", "RLIOP913");
   auto artist = db.get_text("artist");
   if (artist.ok()) {
       std::cout << artist.value() << std::endl;
   }

SQLite relational example:

.. code-block:: c++

   #include "util/db/backends/SqliteBackend.hpp"
   #include "util/db/relational/Database.hpp"

   using Db = PDJE_UTIL::db::relational::RelationalDatabase<
       PDJE_UTIL::db::backends::SqliteBackend>;

   PDJE_UTIL::db::backends::SqliteConfig cfg{
       .path = "tmp/example.sqlite3",
       .open_options = { .create_if_missing = true }
   };

   auto opened = Db::open(cfg);
   if (!opened.ok()) {
       return;
   }

   auto db = std::move(opened.value());
   (void)db.execute(
       "CREATE TABLE IF NOT EXISTS notes("
       "id INTEGER PRIMARY KEY, lane TEXT NOT NULL);");
   (void)db.begin_transaction();
   (void)db.execute("INSERT INTO notes(id, lane) VALUES(?1, ?2);",
                    { PDJE_UTIL::db::relational::Value{ std::int64_t{1} },
                      PDJE_UTIL::db::relational::Value{ PDJE_UTIL::db::Text{"left"} } });
   (void)db.commit();

Annoy nearest-neighbor follows the same open/use/close shape, but its payloads
are `db::nearest::Item`, `SearchHit`, and `SearchOptions` rather than text or
SQL values.

Function Helpers
----------------

The function layer currently splits into small inline helpers plus larger image
and signal-processing utilities.

Scalar And Text
~~~~~~~~~~~~~~~

`PDJE_UTIL::function::clamp` and `slugify` are the smallest maintained public
helpers.

- `clamp(ClampArgs)` clamps a floating-point value and returns
  `Result<double>`
- `slugify(SlugifyArgs, EvalOptions)` normalizes text into a delimiter-based
  slug and rejects alphanumeric separators
- `EvalOptions` currently carries an optional `CacheContext*`
- `CacheContext` exists as a lightweight movable handle; the shipped inline
  helpers shown here do not require a populated cache context

Example:

.. code-block:: c++

   #include "util/PDJE_Util.hpp"

   auto slug = PDJE_UTIL::function::slugify(
       { .input = "Project DJ Engine", .separator = '-' });
   if (slug.ok()) {
       std::cout << slug.value() << std::endl; // "project-dj-engine"
   }

Image Helpers
~~~~~~~~~~~~~

The image surface is split into a generic WebP writer path and a waveform
generator path.

Generic WebP writer:

.. code-block:: c++

   #include "util/function/image/WebpWriter.hpp"

Current public types in that header:

- `RasterPixelFormat`
- `RasterImageView`
- `EncodeWebpArgs`
- `WriteWebpArgs`
- `encode_webp(...)`
- `write_webp(...)`

Current source-backed behavior:

- validates image dimensions, stride, and buffer layout
- accepts `gray8`, `gray_alpha8`, `rgb8`, and `rgba8` raster views
- returns encoded WebP bytes from `encode_webp(...)`
- writes encoded bytes to disk with `write_webp(...)`
- validates `compression_level` in the `[-1, 9]` range before encoding

Minimal example:

.. code-block:: c++

   #include "util/function/image/WebpWriter.hpp"

   const std::vector<std::uint8_t> rgba{ 255, 0, 0, 255 };

   auto encoded = PDJE_UTIL::function::image::encode_webp(
       { .image =
             {
                 .pixels = rgba,
                 .width = 1,
                 .height = 1,
                 .stride = 0,
                 .pixel_format = PDJE_UTIL::function::image::RasterPixelFormat::rgba8,
             } });

Waveform WebP generation:

.. code-block:: c++

   #include "util/function/image/WaveformWebp.hpp"

Current public types in that header:

- `EncodedWebpBytes`
- `ChannelWaveformWebps`
- `WaveformWebpBatch`
- `EncodeWaveformWebpArgs`
- `encode_waveform_webps(...)`

Current source-backed behavior and tests cover:

- argument validation
- interleaved PCM channel splitting
- min/max aggregation per output column
- incomplete chunk padding
- transparent background output
- deterministic output across single-worker, multi-worker, and auto-worker
  runs

Example:

.. code-block:: c++

   #include "util/function/image/WaveformWebp.hpp"

   std::vector<float> pcm{
       1.0f, -1.0f, 0.25f, -0.25f,
       -0.5f, 0.5f, 0.75f, -0.75f
   };

   auto waveform = PDJE_UTIL::function::image::encode_waveform_webps(
       { .pcm = pcm,
         .channel_count = 2,
         .y_pixels = 64,
         .pcm_per_pixel = 256,
         .x_pixels_per_image = 512,
         .compression_level = 1,
         .worker_thread_count = 0 });

   if (!waveform.ok()) {
       return;
   }

   const auto &batch = waveform.value();
   // batch[channel][image] -> encoded WebP bytes

Signal / STFT Helpers
~~~~~~~~~~~~~~~~~~~~~

The maintained STFT surface lives outside `util/PDJE_Util.hpp` and requires a
direct include:

.. code-block:: c++

   #include "util/function/stft/STFT_Parallel.hpp"

The current public interfaces are:

- `PDJE_PARALLEL::BACKEND_T`
- `PDJE_PARALLEL::Backend`
- `PDJE_PARALLEL::STFT`
- `PDJE_PARALLEL::WINDOW_LIST`
- `PDJE_PARALLEL::POST_PROCESS`

The current runtime model from the source tree is:

- `STFT` always constructs a serial backend
- `STFT` calls `Backend::LoadBackend()` and then reads the selected
  `BACKEND_T` through `PrintBackendType()`
- if OpenCL loads successfully, `STFT` tries to construct `OPENCL_STFT`
- if OpenCL startup or execution fails, `STFT` falls back to `SERIAL`
- the current backend loader switches between `OPENCL` and `SERIAL`
- `BACKEND_T::METAL` exists in the enum, but the shipped runtime loader does
  not currently select it

`POST_PROCESS` is the main post-chain contract:

- `to_bin`
- `toPower`
- `mel_scale`
- `to_db`
- `normalize_min_max`
- `to_rgb`

`POST_PROCESS::check_values()` currently applies the chain rules used by the
implementation:

- `to_rgb` implies `normalize_min_max` and `mel_scale`
- `mel_scale` implies `to_bin` and `toPower`

The util tests currently validate:

- serial backend stability across repeated calls
- switching across cached FFT sizes
- mel filter bank generation
- mel, mel+db, and RGB reduction paths
- OpenCL runtime shim caching and backend selection
- CMRC resource packaging for `STFT_MAIN.cl`

Example:

.. code-block:: c++

   #include "util/function/stft/STFT_Parallel.hpp"

   std::vector<float> pcm(512, 0.0f);

   PDJE_PARALLEL::STFT stft;
   PDJE_PARALLEL::POST_PROCESS post;
   post.mel_scale = true;
   post.to_db = true;
   post.check_values();

   auto [real_out, imag_out] = stft.calculate(
       pcm,
       PDJE_PARALLEL::WINDOW_LIST::HANNING,
       10,
       0.5f,
       post);

If you need to probe runtime availability directly, the loader entrypoint lives
in:

.. code-block:: c++

   #include "util/common/BackendLoader/OpenCL_Loader.hpp"

And the capability query is:

.. code-block:: c++

   bool ready = PDJE_PARALLEL::EnsureOpenCLRuntimeLoaded();

AI Namespace
------------

`PDJE_UTIL::ai` currently exists as a placeholder namespace only. The current
header set does not define public AI utility functions inside it.
