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

Clone and build PDJE in a few steps:

.. code-block:: bash

   git clone https://github.com/Rliop913/Project_DJ_Engine.git
   cd Project_DJ_Engine
   mkdir build && cd build
   cmake ..
   cmake --build . --parallel

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

--------------------

.. toctree::
  :maxdepth: 2
  :caption: Documentation:

  Getting Started
  How_To_Use
  api/api_root
  PDJE_Format
