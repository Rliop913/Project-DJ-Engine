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

`db::OpenOptions` is shared across the shipped backends and controls three
important behaviors:

- `create_if_missing`
  create the underlying database if the target path does not exist yet
- `truncate_if_exists`
  delete and recreate the target before opening it
- `read_only`
  open an existing database without permitting writes

`db::Database<Backend>` is still available as a convenience alias for
key-value access, but the more explicit `keyvalue::KeyValueDatabase<Backend>`
and `relational::RelationalDatabase<Backend>` names are clearer in new docs and
new call sites.

The utility tests in the current tree show two practical patterns:

- use `RocksDbBackend` when the workload is key/value oriented
- use `SqliteBackend` when the workload is SQL oriented

`AnnoyBackend` and `db::nearest::NearestNeighborIndex<Backend>` are still part
of the public capability set, but this page only touches them briefly because
the current engine-facing and example-facing usage is dominated by SQLite and
RocksDB.

DB Access API
~~~~~~~~~~~~~

Low-Level C++
^^^^^^^^^^^^^

At the low-level utility layer, the wrapper classes all follow the same
lifecycle:

- `create(config)`
- `open(config)`
- use the database object
- `close()`
- optionally `destroy(config)` when the backing store should be removed

The public wrapper split is:

.. list-table::
   :header-rows: 1
   :widths: 25 45 30

   * - Wrapper
     - Main use
     - Typical backend
   * - `db::keyvalue::KeyValueDatabase<Backend>`
     - text/blob lookups by key
     - `db::backends::RocksDbBackend`
   * - `db::relational::RelationalDatabase<Backend>`
     - SQL execution, queries, and transactions
     - `db::backends::SqliteBackend`
   * - `db::nearest::NearestNeighborIndex<Backend>`
     - embedding search and ANN indexing
     - `db::backends::AnnoyBackend`

For key-value code, the important value types are:

- `db::Key`
- `db::Text`
- `db::Bytes`

For relational code, the important data carriers are:

- `db::relational::Value`
- `db::relational::Params`
- `db::relational::Row`
- `db::relational::ExecResult`
- `db::relational::QueryResult`

RocksDB key-value usage:

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

   auto artist = db.get_text("artist");
   if (artist.ok()) {
       std::cout << artist.value() << std::endl;
   }

The key-value wrapper also exposes `contains()`, `erase()`, and `list_keys()`
for simple metadata stores and blob caches.

SQLite relational usage:

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
   (void)db.execute(
       "CREATE TABLE IF NOT EXISTS notes("
       "id INTEGER PRIMARY KEY, "
       "lane TEXT NOT NULL)");

   (void)db.execute("INSERT INTO notes(id, lane) VALUES(?1, ?2);",
                    { db::relational::Value{std::int64_t{1}},
                      db::relational::Value{db::Text{"left"}} });

   auto rows = db.query("SELECT lane FROM notes WHERE id = ?1;",
                        { db::relational::Value{std::int64_t{1}} });
   if (rows.ok() && !rows.value().rows.empty()) {
       const auto *lane = rows.value().rows.front().find("lane");
       if (lane != nullptr) {
           std::cout << std::get<db::Text>(lane->storage) << std::endl;
       }
   }

`RelationalDatabase<SqliteBackend>` also exposes `begin_transaction()`,
`commit()`, and `rollback()`. The utility tests explicitly cover successful SQL
queries, parameter binding, blob transport, transaction rollback/commit, and
read-only open mode.

Godot API
^^^^^^^^^

In `PDJE-Godot-Plugin-cloned`, the Godot-facing DB story is split into two
layers:

- `PDJE_LowLevelUtilAPI`
  direct util-wrapper access for key-value, relational, nearest-neighbor, and
  waveform helper entrypoints
- `PDJE_Wrapper` plus `EditorWrapper`
  higher-level engine and root-DB workflows for authored PDJE content

`PDJE_LowLevelUtilAPI` is the low-level Godot bridge for util DB wrappers. Its
methods mirror the C++ lifecycle and return a normalized `Dictionary` envelope:

- `ok`
- `code`
- `message`
- `data`

The current bridge exposes:

- `KeyValueCreate`, `KeyValueOpen`, `KeyValueClose`, `KeyValueGetText`,
  `KeyValueGetBytes`, `KeyValuePutText`, `KeyValuePutBytes`, `KeyValueErase`,
  `KeyValueListKeys`
- `RelationalCreate`, `RelationalOpen`, `RelationalClose`,
  `RelationalExecute`, `RelationalQuery`, `RelationalBeginTransaction`,
  `RelationalCommit`, `RelationalRollback`
- `NearestCreate`, `NearestOpen`, `NearestSearch`, and related ANN helpers

Low-level util-style usage in Godot therefore looks like this:

.. code-block:: gdscript

   var util = PDJE_LowLevelUtilAPI.new()

   print(util.KeyValueOpen({
       "path": "res://imgdb",
       "create_if_missing": true
   }))
   print(util.KeyValuePutText("artist", "RLIOP913"))

   var rows = util.RelationalQuery(
       "SELECT ?1 AS lane;",
       ["left"]
   )
   print(rows)

The `data` payload varies by method. For example:

- key-value payloads include fields such as `text`, `bytes`, `keys`, and
  `is_open`
- relational query payloads include `rows`, where each row carries `columns`
  and `values`
- relational execute payloads include `affected_rows` and
  `last_insert_rowid`

When the task is not generic DB access but authored engine content, the higher
level Godot path still goes through the engine wrapper layer:

- `PDJE_Wrapper.InitEngine(path)`
- `PDJE_Wrapper.SearchMusic(title, composer, bpm)`
- `PDJE_Wrapper.SearchTrack(title)`
- `PDJE_Wrapper.GetEditor()`
- `EditorWrapper.render(track_title)`
- `EditorWrapper.pushToRootDB(music_title, music_composer)`
- `EditorWrapper.pushTrackToRootDB(track_title)`

Current wrapper-style root-DB usage from the Godot example project looks like
this:

.. code-block:: gdscript

   var engine = PDJE_Wrapper.new()
   engine.InitEngine("res://rootdb")

   if engine.SearchMusic("title", "composer").is_empty():
       engine.InitEditor("name", "none", "res://sandbox")
       var editor = engine.GetEditor()
       editor.ConfigNewMusic("title", "composer", "G://YMCA.wav")
       print(editor.render("sample_track"))
       print(editor.pushToRootDB("title", "composer"))

   var tracks = engine.SearchTrack("sample_track")
   print(tracks)

This distinction is important:

- use `PDJE_LowLevelUtilAPI` when you want direct util DB semantics
- use `PDJE_Wrapper` and `EditorWrapper` when you want PDJE music, track, and
  editor workflows backed by the root DB
- nearest-neighbor methods are available in the low-level util bridge, but they
  remain secondary to the SQLite and RocksDB scenarios emphasized on this page

Function Helpers
----------------

`PDJE_UTIL::function` currently contains small, composable helpers.

- `clamp(ClampArgs)` clamps a floating-point value inside a range.
- `slugify(SlugifyArgs, EvalOptions)` normalizes text into a delimiter-based
  slug.
- `function::image` contains image-oriented helpers such as WebP writers and
  waveform image generation.

There are also Halide-oriented helper entrypoints in the tree, but this page
does not use the Halide sample code as its teaching path. The focus here stays
on the utility APIs that are already exercised by public headers, unit tests,
and wrapper-facing workflows.

Example:

.. code-block:: c++

   using namespace PDJE_UTIL;

   auto slug = function::slugify({ .input = "Project DJ Engine", .separator = '-' });
   if (slug.ok()) {
       std::cout << slug.value() << std::endl; // "project-dj-engine"
   }

SoundToWaveForm
~~~~~~~~~~~~~~~

The waveform-image path lives under `PDJE_UTIL::function::image` and is exposed
through:

.. code-block:: c++

   #include "util/function/image/WaveformWebp.hpp"

This is an important include detail: `encode_waveform_webps()` is currently
declared in `util/function/image/WaveformWebp.hpp`, and it is not re-exported
by `util/PDJE_Util.hpp`.

Low-Level C++
^^^^^^^^^^^^^

The public entrypoints are:

- `PDJE_UTIL::function::image::EncodeWaveformWebpArgs`
- `PDJE_UTIL::function::image::encode_waveform_webps`
- `PDJE_UTIL::function::image::WaveformWebpBatch`

`EncodeWaveformWebpArgs` is the full call contract:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Field
     - Meaning
   * - `pcm`
     - input PCM sample span; expected as interleaved float samples
   * - `channel_count`
     - number of channels in the interleaved PCM input
   * - `y_pixels`
     - waveform image height in pixels
   * - `pcm_per_pixel`
     - number of PCM samples reduced into one x-axis column
   * - `x_pixels_per_image`
     - image width in pixels for each emitted waveform tile
   * - `compression_level`
     - WebP compression level from `-1` to `9`
   * - `worker_thread_count`
     - explicit worker count; `0` means auto-select

The return type is a nested batch:

- `WaveformWebpBatch`
  `batch[channel][image]`
- each leaf value is an encoded WebP byte array for one channel tile

Operationally, the implementation does the following:

- validates that the PCM span and shape parameters are usable
- splits interleaved PCM into per-channel vectors
- pads incomplete channel tails with zeroes when a chunk is short
- groups each output image into `pcm_per_pixel * x_pixels_per_image` samples
- reduces each x-axis column to min/max sample values
- maps those min/max values into pixel rows
- draws opaque white waveform strokes on a transparent RGBA background
- encodes each generated tile into WebP bytes
- runs jobs across multiple workers when `worker_thread_count` is greater than
  `1`, or chooses a hardware-based worker count when it is `0`

The waveform tests also verify several practical behaviors:

- invalid required arguments return `StatusCode::invalid_argument`
- channel splitting preserves per-channel output layout
- min/max aggregation fills vertical spans inside a column
- incomplete chunks are padded before reduction
- single-worker and multi-worker runs produce the same output bytes

Example:

.. code-block:: c++

   using namespace PDJE_UTIL;

   std::vector<float> pcm {
       1.0f, -1.0f, 0.25f, -0.25f,
       -0.5f, 0.5f, 0.75f, -0.75f
   };

   auto encoded = function::image::encode_waveform_webps(
       { .pcm = pcm,
         .channel_count = 2,
         .y_pixels = 64,
         .pcm_per_pixel = 256,
         .x_pixels_per_image = 512,
         .compression_level = 1,
         .worker_thread_count = 0 });

   if (!encoded.ok()) {
       std::cerr << encoded.status().message << std::endl;
       return;
   }

   const auto &batch = encoded.value();
   // batch[channel][image] -> encoded WebP bytes

Godot API
^^^^^^^^^

`PDJE-Godot-Plugin-cloned` exposes waveform functionality in two Godot-facing
layers, and both are registered as public classes:

- `PDJE_LowLevelUtilAPI`
- `PDJE_HighLevelUtilAPI`

The low-level Godot entrypoint is:

- `PDJE_LowLevelUtilAPI.EncodeWaveformWebps(...)`

This method mirrors the native `encode_waveform_webps()` call but accepts Godot
types such as `Array` and returns the same normalized result envelope used by
the other low-level util bridges:

- `ok`
- `code`
- `message`
- `data`

For waveform generation, `data` includes:

- `images`
  a flattened `Array` of `PackedByteArray` WebP payloads
- `layout`
  currently `"channel_major"`
- `channel_count`
- `images_per_channel`
- `total_images`
- `y_pixels`
- `pcm_per_pixel`
- `x_pixels_per_image`
- `compression_level`
- `worker_thread_count`
- `pcm_length`

Low-level Godot usage looks like this:

.. code-block:: gdscript

   var util = PDJE_LowLevelUtilAPI.new()
   var result = util.EncodeWaveformWebps(
       [1.0, -1.0, 0.25, -0.25],
       2,
       256,
       48,
       4096,
       -1,
       0
   )

   if result["ok"]:
       var images = result["data"]["images"]
       print(images.size())

The higher-level Godot entrypoint is:

- `PDJE_HighLevelUtilAPI.SoundToWaveform(core_api, keyvalue_db_path, music_item, pcm_per_pixel, width, height, start_index, end_index)`

This high-level adapter does more than simple forwarding:

- reads PCM by calling `PDJE_Wrapper.GetPCMFromMusicData(music_item)`
- encodes waveform WebPs through the low-level waveform bridge
- caches the generated image array in a RocksDB key-value store at
  `keyvalue_db_path`
- slices the cached/generated image list using `start_index` and `end_index`

Current high-level waveform usage from `util_test.tscn` looks like this:

.. code-block:: gdscript

   var core = PDJE_Wrapper.new()
   core.InitEngine("res://rootdb")

   var util = PDJE_HighLevelUtilAPI.new()
   var music = core.SearchMusic("", "")[0]
   var images = util.SoundToWaveform(
       core,
       "res://imgdb",
       music,
       48,
       4096,
       256,
       0,
       -1
   )

   var image = Image.new()
   image.load_webp_from_buffer(images[0])

The practical split is:

- use `PDJE_LowLevelUtilAPI.EncodeWaveformWebps()` when you already have PCM and
  want a thin wrapper over the util module
- use `PDJE_HighLevelUtilAPI.SoundToWaveform()` when you want the Godot wrapper
  to fetch PCM from music metadata and manage cached waveform images for you

AI Namespace
------------

`PDJE_UTIL::ai` currently exists as a placeholder namespace only. There are no
public AI utility functions defined in the current header set.

Generated API
-------------

Use :doc:`/api/api_root` if you need the file-by-file generated view of utility
headers and backend classes.
