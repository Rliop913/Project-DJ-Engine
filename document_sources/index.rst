Project_DJ_Engine – A Real‑Time Engine for Rhythm Games, DJing, and Audio Production
======================================================================================

**Project_DJ_Engine (PDJE)** is an integrated, production‑ready engine combining rhythm game mechanics, DJ performance tools, and digital audio workstation features into a single soft real‑time C++ system. It lets you create, reproduce, and remix DJ mixes, drive rhythm‑game note charts, and build interactive music experiences—all with low‑latency, millisecond‑level timing guarantees.

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

PDJE is organized into independent, dynamically loadable modules:

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
  
  Available on linux, windows
- **Note Engine** :
  This engine is designed to handle rhythm game logic or real-time timing judgments.
  It can be used by connecting either the PDJE Input Engine or another input engine wrapped for compatibility.

  The timing resolution is determined when initializing the PDJE Core Engine.
  By default, the PDJE Core Engine plays audio at a sample rate of 48,000 Hz, meaning it processes 48,000 samples per second.

  Assuming the buffer size is set to 48 samples, the core updates its internal sample-accumulation counter 1,000 times per second (48,000 / 48).

  This results in a theoretical judgment resolution of 1,000 counts per second, or 1 millisecond per count,
  giving you a minimum resolution of approximately 1 ms + input latency.
- **Utility Engine** :
  This engine handles non-essential but highly useful functionalities beyond the core features.

  It can be integrated with the various modules mentioned above to provide optional capabilities such as AI inference, FFT processing, and music analysis using advanced algorithms.

  Some of these features may require GPU acceleration or SIMD support, and could demand high computational performance depending on the workload.

MileStones
------------

.. mermaid::
  :config: {"theme":"forest","themeVariables":{"fontSize":"25px"}}

  timeline
    section DJ + DAW Rhythm Engine
      0.5.0 : Core Module Implemented
      0.7.0 : Rhythm Game Judging Module Implemented
      0.9.0 : CrossPlatform Input & MIDI Module Implemented
      1.0.0 : Stable Release
    section DJ + DAW + HPC + AI Rhythm Engine
      1.2.0 : Basic Utility Module Implemented
      1.5.0 : OnnxRuntime Integration Utility Module
      2.0.0 : OnnxRuntime Integrated with OKL (OCCA Kernel Language)
            : Cross-Vendor GPGPU Support Enabled





Use Cases
---------

PDJE is ideal for:

- **Custom rhythm‑game development** with built‑in mixing
- **Realtime + Pre-made DJ performance**
- **In‑game music editors and DAW** for dynamic chart and mixset creation
- **Low Latency Input** for linux(io_uring) and windows(rawinput).
  
Additional Resources
--------------------

- 💻 **GitHub**: https://github.com/Rliop913/Project_DJ_Engine  
- 📄 **License**: LGPLv2.1 (by SoundTouch License)
- 🎮 **Godot Plugin**: https://github.com/Rliop913/PDJE_Godot_Plugin
- 📦 **Plugin Prebuilt**: https://github.com/Rliop913/Project_DJ_Godot
  
--------------------

CI/CD Call Graph
-------------------

.. mermaid:: 
  
  flowchart TD
  Project_DJ_Engine --> PDJE_Godot_Plugin
  PDJE_Godot_Plugin --> Project_DJ_Godot

  origin_build_success --> Project_DJ_Engine
  manual_release --> PDJE_Godot_Plugin



.. toctree::
  :maxdepth: 1
  :caption: Documentation:

  Getting Started
  api/api_root
  Editor_Format
