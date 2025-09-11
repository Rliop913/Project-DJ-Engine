Data_Lines
================


Overview
----------

A ``Data Line`` is a lightweight transport lane that exposes the **addresses of the engine’s core state variables** to external modules.  
It lets you read values such as frame/cursor positions and pre-rendered buffers **without copying** (via simple pointer dereference), enabling high-performance synchronization with visualizers, editor UIs, external runtimes, and plugins. The **Core Data Line** is the standard lane that carries the **minimal common set** managed directly by the engine core.

Key properties
^^^^^^^^^^^^^^

- **Zero-copy access**: Values are provided as **non-owning pointers**.
- **Low latency**: Poll or callback and dereference immediately to read the newest state.

When to use
^^^^^^^^^^^

- Driving **timeline UIs** from playback/max cursors
- **Waveform preview, level meters, spectrograms**, and other real-time visualizations
- **Lightweight interactions** with FX/Music controllers (for full control, prefer each panel’s API)

Lifetime and threading
^^^^^^^^^^^^^^^^^^^^^^

- All members point to memory **owned by the engine**. Re-initialization/reset may change addresses; after such events, perform a **null check** and call ``PullOutDataLine()`` again to refresh the handles.
- Updates may occur on audio/engine threads. If polling from other threads, **read briefly and copy locally**; avoid holding pointers for long periods. If needed, use a **double-buffer/snapshot** pattern on the consumer side.

Recommended usage
^^^^^^^^^^^^^^^^^

- **Read-mostly**: Treat cursors/frames/buffers as primarily *read-only*.  
  Even if mutation is technically possible, for stability/portability **use the ``FXControlPannel`` / ``MusicControlPannel`` APIs** to change state.
- **Null/size checks**: If a pointer is ``nullptr`` or the buffer length is unknown, do not dereference; use the engine/wrapper’s length/query APIs.
- **ABI caution**: The struct layout may evolve. Check the documented versions (``PDJE_VERSION``, ``PDJE_WRAPPER_VERSION``).

.. warning::

   A Data Line is a **set of non-owning pointers**. Do not free memory or perform operations that can introduce write races.  
   In concurrent environments, capture short snapshots within a small critical section; route long-lived changes through the appropriate panel APIs.


Core Data Line
----------------

.. doxygenfunction:: PDJE::PullOutDataLine
    :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_CORE_DATA_LINE
    :project: Project_DJ_Engine

.. tab-set-code:: 

    .. code-block:: c++

        auto engine = new PDJE("database/path");
        PDJE_CORE_DATA_LINE engineLine = engine->PullOutDataLine();


    .. code-block:: c#

        PDJE engine = new PDJE("database/path");
        var engineLine = engine.PullOutDataLine();

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        engine = pypdje.PDJE("database/path")
        engineLine = engine.PullOutDataLine()

    .. code-block:: gdscript

        var engine:PDJE_Wrapper = PDJE_Wrapper.new()
        engine.InitEngine("res://database/path")
        var engineLine:CoreLine = engine.PullOutCoreLine()

