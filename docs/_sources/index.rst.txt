Project-DJ-Engine – A Real‑Time Engine for Rhythm Games, DJing, and Audio Production
======================================================================================

**Project-DJ-Engine (PDJE)** is a soft real-time C++ engine that combines rhythm game logic, DJ performance workflows, and DAW-style editing APIs. It lets you create, reproduce, and remix DJ mixes, drive rhythm-game note charts, and build interactive music experiences with low-latency pipelines and synchronized high-resolution timestamps.

PDJE’s modular core provides:

- **High‑performance audio processing** and time synchronization  
- **Real‑time DJ mixing**
- **Integrated editor suite** for music metadata, mixset & note‑chart authoring  
- **Centralized data management** for tracks, metadata, mixsets, and note-chart datas
- **Live interaction** via MIDI, dynamic input, and automation APIs  

Quick Start
-----------

Prerequisites:

- C++20 compatible compiler
- CMake 3.12 or later
- SWIG
- OpenSSL
- [Conan](https://conan.io/)

Clone and build PDJE in a few steps:
on linux & macos

.. code-block:: bash
  
  bash ./BuildInitwithConan.sh . Release
  mkdir build
  cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="../conan_cmakes/conan_toolchain.cmake" -DPDJE_DYNAMIC=OFF #to get dynamic library, change here
  cmake --build . --parallel #add your maximum number of cores

on windows

.. code-block:: bash  

  ./BuildInitwithConan.bat . static Release
  #to get dynamic library, change static into dynamic
  mkdir build
  cd build
  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="../conan_cmakes/conan_toolchain.cmake" -DPDJE_DYNAMIC=OFF #to get dynamic library, change here
  cmake --build . --config Release --parallel #add your maximum number of cores

.. warning:: 
  To change the build type (Debug/Release) or switch between static and dynamic builds,  
  you must re-run the `BuildInitwithConan` script with the new options.  
  Otherwise, Conan dependencies may not be configured correctly.


Learn about PDJE's modules and usage in the :doc:`Getting Started` documentation.


System Architecture
-------------------

PDJE is organized into major modules. In the default build these are linked together through the project targets, and selected modules can also be built as separate libraries depending on CMake options.

- **Core Engine** : 
  The Core Engine is the most critical component of the PDJE project.

  It builds, queries, and modifies the core database required for engine operation, storing not only mixset data, note charts, and music metadata, but even the audio tracks themselves.

  Mixsets are converted into structured data and preprocessed into fully playable tracks before playback.

  During playback, new audio content from the database can be dynamically added to a mixset in real time.

  All audio tracks and mixsets being played can be fully controlled and processed with effects in real time.
  (Even without a mixset, the engine can operate in Full Manual Mode — allowing for truly live DJing.)

  The Core Engine also includes an integrated editor.
  It allows users to register and index music, create mixsets, and generate note charts typically used in rhythm games.

  All modifications are tracked and recorded using Git, enabling full version control and the ability to revert to any previous state at any time.

  For smoother integration (and to save developers from too much suffering), the editor is exposed as an API and runs in its own sandboxed playground.
- **Input Engine** :
  It is a cross-platform input engine designed to minimize latency by handling input at the lowest deployable level. 
  
  It supports general-purpose inputs such as keyboard and mouse, as well as MIDI input.
  
  Available on Linux and Windows in the current source tree. macOS support is planned.
- **Judge Engine** :
  

  The Judge Engine handles rhythm-game timing logic and real-time judgments.
  It can run with the PDJE Input Engine or with any compatible, wrapped input source.

  **High-resolution timebase (update)**

  The input module timestamps each input event, and the core engine timestamps each audio callback using the same monotonic, high-resolution clock.
  By synchronizing these timebases, the judge computes the time difference between an input event and the expected note time with **microsecond-level accuracy**.

  **What changed**

  * Judgment resolution is no longer limited by the audio buffer cadence.
  * Sample rate and buffer size still affect audio scheduling and overall latency, but **they do not cap judgment precision**.

  **Audio cadence (context)**

  By default, the PDJE Core Engine runs at 48,000 Hz.
  With a 48-sample buffer, the audio callback cadence is 1,000 Hz (48,000 / 48).
  Previously this implied ~1 ms timing steps; now the judge uses synchronized timestamps for microsecond-level precision and applies audio/buffer latency compensation separately.

  **Practical notes**

  * Input and audio threads share the same monotonic clock (e.g., Windows `QPC`, Linux `CLOCK_MONOTONIC`/`CLOCK_MONOTONIC_RAW`).
  * Initial synchronization (with periodic drift checks) aligns the input and audio epochs so the computed time differences reflect true inter-event timing.

- **Utility Engine** :
  The Utility Engine page currently represents roadmap-level ideas (for example, AI/analysis helpers). There is no active `PDJE_MODULE_UTILITY` build target in the current CMake configuration.

MileStones
------------

.. mermaid::
  :config: {"theme":"forest","themeVariables":{"fontSize":"25px"}}

  timeline
    section DJ + DAW Rhythm Engine
      0.5.0 : Core Module Implemented
      0.6.0 : PDJE_Input module implement-Windows
      0.7.0 : PDJE_Judge module implement
      0.8.0 : PDjE_Input module implement-Linux, MIDI
      0.9.0 : PDJE_Input module implement-Mac (planned)
      1.0.0 : Stable Release
    section DJ + DAW + HPC + AI Rhythm Engine
      1.2.0 : Basic Utility Module (roadmap)
      1.5.0 : OnnxRuntime Integration Utility Module
      2.0.0 : OnnxRuntime Integrated with AdaptiveCPP
            : Cross-Vendor GPGPU Support Enabled


See: AdaptiveCPP (https://github.com/AdaptiveCpp/AdaptiveCpp)

Use Cases
---------

PDJE is ideal for:

- **Custom rhythm‑game development** with built‑in mixing
- **Realtime + Pre-made DJ performance**
- **In‑game music editors and DAW** for dynamic chart and mixset creation
- **Low Latency Input** for Linux (epoll-based path) and Windows (Raw Input).
  
Additional Resources
--------------------

- **GitHub**: https://github.com/Rliop913/Project-DJ-Engine  
- **License**: LGPL v2.1
- **Godot Plugin**: https://github.com/Rliop913/PDJE-Godot-Plugin
- **Plugin Prebuilt**: https://github.com/Rliop913/Project_DJ_Godot
  
--------------------

CI/CD Call Graph
-------------------

  .. :config: {"theme":"forest","themeVariables":{"fontSize":"25px"}}

.. mermaid::

  %%{init: {'flowchart': {'curve': 'stepAfter'}}}%%

  flowchart TD
  subgraph CORE_DEVELOP
    push_to_core/dev --> core/dev
    core/dev --> core/dev_build_test
    core/dev_build_test --> core/main
    core/main --> core/main_build_test
  end

  subgraph VALID_CHECK
    RELEASE --> self_clone/git_lfs_test
    self_clone/git_lfs_test --> PASS
    self_clone/git_lfs_test --> FAIL
    FAIL --> revert_commit
  end
  subgraph WRAPPER_DEVELOP
    push_to_wrapper/dev --> wrapper/dev
    wrapper/dev --> wrapper/dev_build_test
    core/dev --> wrapper/dev_build_test
    wrapper/dev_build_test --> wrapper/main
    wrapper/main --> wrapper/main_build_test
    core/main --> wrapper/main_build_test
  end

  CORE_DEVELOP --> Project_DJ_Engine
  WRAPPER_DEVELOP --> PDJE_Godot_Plugin
  Project_DJ_Engine -->|TRIG_CICD| PDJE_Godot_Plugin
  PDJE_Godot_Plugin -->|RELEASE| Project_DJ_Godot
  Project_DJ_Godot --> VALID_CHECK




.. toctree::
  :maxdepth: 1
  :caption: Documentation:

  Getting Started
  api/api_root
  Editor_Format
