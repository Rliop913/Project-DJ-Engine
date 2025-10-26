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
  Even if mutation is technically possible, for stability/portability **use the ``FXControlPanel`` / ``MusicControlPanel`` APIs** to change state.
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
        auto core_line = engine->PullOutDataLine();
        core_line.preRenderedData;
        // Pre-rendered PCM audio data (float32 array).
        // Interleaved stereo format: [L0, R0, L1, R1, L2, R2, ...]
        // To access a specific frame's left/right channels:
        //   Left  = preRenderedData[frameIndex * 2]
        //   Right = preRenderedData[frameIndex * 2 + 1]

        core_line.maxCursor;
        // Total number of PCM frames per channel.
        // For stereo, preRenderedData will contain (maxCursor * 2) float samples.

        core_line.nowCursor;
        // Current playback position in PCM frames (per channel).
        // This value can jump if seeking occurs.
        // Multiply by 2 to index into preRenderedData (stereo interleaving).

        core_line.used_frame;
        // Total number of PCM frames actually played (accumulated).
        // Does not decrease when seeking; represents playback progress.

    .. code-block:: c#

        PDJE engine = new PDJE("database/path");
        var core_line = engine.PullOutDataLine();
        core_line.preRenderedData;
        // Pre-rendered PCM audio data (float32 array).
        // Interleaved stereo format: [L0, R0, L1, R1, L2, R2, ...]
        // To access a specific frame's left/right channels:
        //   Left  = preRenderedData[frameIndex * 2]
        //   Right = preRenderedData[frameIndex * 2 + 1]

        core_line.maxCursor;
        // Total number of PCM frames per channel.
        // For stereo, preRenderedData will contain (maxCursor * 2) float samples.

        core_line.nowCursor;
        // Current playback position in PCM frames (per channel).
        // This value can jump if seeking occurs.
        // Multiply by 2 to index into preRenderedData (stereo interleaving).

        core_line.used_frame;
        // Total number of PCM frames actually played (accumulated).
        // Does not decrease when seeking; represents playback progress.


    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        engine = pypdje.PDJE("database/path")
        core_line = engine.PullOutDataLine()
        core_line.preRenderedData
        # Pre-rendered PCM audio data (float32 array).
        # Interleaved stereo format: [L0, R0, L1, R1, L2, R2, ...]
        # To access a specific frame's left/right channels:
        #   Left  = preRenderedData[frameIndex * 2]
        #   Right = preRenderedData[frameIndex * 2 + 1]

        core_line.maxCursor
        # Total number of PCM frames per channel.
        # For stereo, preRenderedData will contain (maxCursor * 2) float samples.

        core_line.nowCursor
        # Current playback position in PCM frames (per channel).
        # This value can jump if seeking occurs.
        # Multiply by 2 to index into preRenderedData (stereo interleaving).

        core_line.used_frame
        # Total number of PCM frames actually played (accumulated).
        # Does not decrease when seeking; represents playback progress.


    .. code-block:: gdscript

        var engine:PDJE_Wrapper = PDJE_Wrapper.new()
        engine.InitEngine("res://database/path")
        var core_line = engine.PullOutCoreLine()

        core_line.GetPreRenderedFrames()
        # Pre-rendered PCM audio data (float32 array).
        # Interleaved stereo format: [L0, R0, L1, R1, L2, R2, ...]
        # To access a specific frame's left/right channels:
        #   Left  = preRenderedData[frameIndex * 2]
        #   Right = preRenderedData[frameIndex * 2 + 1]

        core_line.GetMaxCursor()
        # Total number of PCM frames per channel.
        # For stereo, preRenderedData will contain (maxCursor * 2) float samples.

        core_line.GetNowCursor()
        # Current playback position in PCM frames (per channel).
        # This value can jump if seeking occurs.
        # Multiply by 2 to index into preRenderedData (stereo interleaving).

        core_line.GetUsedFrame()
        # Total number of PCM frames actually played (accumulated).
        # Does not decrease when seeking; represents playback progress.


Input Data Line
-----------------


.. doxygenfunction:: PDJE_Input::PullOutDataLine
    :project: Project_DJ_Engine

.. doxygenstruct:: PDJE_INPUT_DATA_LINE
    :project: Project_DJ_Engine

.. tab-set-code:: 

    .. code-block:: c++

        auto dline = pdje_input_object.PullOutDataLine();
        pdje_input_object.Run();
        while (true) {
            auto got = dline.input_arena->Get();

            for (const auto &i : *got) {
                auto name = dline.id_name_conv->find(i.id);
                if (name != dline.id_name_conv->end()) {
                    std::cout << "name: " << name->second << std::endl;
                    std::cout << "time: " << i.microSecond << std::endl;
                    if(i.type == PDJE_Dev_Type::KEYBOARD){
                        std::cout
                        << "keyNumber: " << static_cast<int>(i.event.keyboard.k)
                        << std::endl;//check PDJE_KEY::
                        std::cout << "pressed" << i.event.keyboard.pressed
                        << std::endl;
                    }

                }
            }
        }

    .. code-block:: c#

        //no impl

    .. code-block:: python

        #no impl


    .. code-block:: gdscript

        #no impl