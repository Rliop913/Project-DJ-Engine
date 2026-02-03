# Gemini Agent Prompt — Recursive Documentation Builder

## 0) Mission
You are a **documentation agent**. Your job is to read this project’s code + existing docs, then **design a documentation strategy** and **continuously refine that strategy and summary by updating ONLY this file (`gemini.md`)**.

The goal is to make `gemini.md` evolve into a complete, actionable documentation plan + project summary that can later drive real documentation work.

---

**Project Summary:**

"Project-DJ-Engine" (PDJE) is an integrated, production-ready engine combining rhythm game mechanics, DJ performance tools, and digital audio workstation features into a single soft real-time C++ system. It lets you create, reproduce, and remix DJ mixes, drive rhythm-game note charts, and build interactive music experiences—all with low-latency, microsecond-level timing guarantees.

**Documentation Strategy Summary:**

The documentation strategy is to enhance the existing Doxygen and Sphinx documentation. This involves first correcting identified discrepancies between the documentation and the code, then updating the existing `.rst` source files in `document_sources/` with more detailed and accurate information, ensuring the Doxygen C++ API reference is up-to-date, and creating new `.rst` files for tutorials and other missing content as needed, following the established structure. The goal is to produce a comprehensive and high-quality documentation site that is easy to navigate and understand for all users.

---

## 1) Non-Negotiable Rules (Hard Constraints)
... (Rules remain the same) ...

---

## 2) What “Recursive” Means Here
... (Recursive loop definition remains the same) ...

---

## 3) Working Principles / Quality Bar
... (Working principles remain the same) ...

---

## 4) Mandatory Output Format Inside `gemini.md`
When you update this file, keep (and improve) the following sections in order.

### 4.1 Project Snapshot (keep concise)
-   **Project name**: Project-DJ-Engine (PDJE)
-   **One-liner**: An integrated, production-ready engine combining rhythm game mechanics, DJ performance tools, and digital audio workstation features into a single soft real-time C++ system.
-   **Target users**: Developers interested in building custom rhythm-games, real-time DJ performance tools, or in-game music editors.
-   **Primary value**: Provides a C++ engine with high-performance audio processing, real-time DJ mixing, an integrated editor suite, and centralized data management.
-   **Key features**:
    *   High-performance audio processing and time synchronization.
    *   Real-time DJ mixing.
    *   Integrated editor suite for music metadata, mixset & note-chart authoring.
    *   Centralized data management for tracks, metadata, mixsets, and note-chart datas.
    *   Live interaction via MIDI, dynamic input, and automation APIs.
    *   Modular architecture with `Core`, `Input`, `Judge`, and `Utility` engines.
-   **Non-goals**: No graphics-related technology is built-in.

### 4.2 Architecture Overview
-   **High-level diagram (text)**:
    ```
  +-------------------+        +-------------------+        +-------------------+
  | Source (include/) |------> | Core Library (C++)|------> | Applications      |
  | (headers & impl)  |        |  (PDJE)           |        | (Godot plugin,    |
  |                   |        |                   |        |  custom C++)      |
  +-------------------+        +-------------------+        +-------------------+
            ^                            |
            |                            |
  +-------------------+        +-------------------+
  | Build System      |        | Conan Packages    |
  | (CMake, Conan)    |<-------| (Dependencies)    |
  +-------------------+        +-------------------+

  (Optional Modules built on Core)
  +-------------------+        +-------------------+
  | PDJE_MODULE_JUDGE |        | PDJE_MODULE_INPUT |
  +-------------------+        +-------------------+
  | PDJE_MODULE_UTILITY |
  +---------------------+
  ```
-   **Modules**:
    -   `document_sources/`: Source files (`.rst`, `conf.py`) for the Sphinx documentation.
    -   `docs/`: Generated HTML documentation output directory.
    -   `include/`: Primary location for all C++ source code.
    -   `Core Engine`: Builds, queries, and modifies the core database; handles audio processing, real-time mixing, and an integrated editor with Git-based version control.
    -   `Input Engine`: Cross-platform, low-latency input handling for keyboard, mouse, and MIDI.
    -   `Judge Engine`: Handles rhythm-game timing logic and real-time judgments with microsecond-level accuracy.
    -   `Utility Engine`: Handles non-essential but useful functionalities like AI inference, FFT processing, and music analysis.
-   **Key data flows**:
    *   User creates `PDJE` object -> `SearchMusic`/`SearchTrack` populates data -> `InitPlayer` sets up audio -> `player` handles music/FX.
    *   User creates `PDJE` object -> `InitEditor` sets up editor -> `editor` manages track/mix/note data.
-   **Key abstractions / interfaces**: The `PDJE` class is the main interface. `audioPlayer`, `editorObject`, `trackdata`, `litedb`, and various `EDIT_ARG_*` types are key abstractions.

### 4.3 Build / Run / Dev Workflow
-   **Prereqs**: C++20 compatible compiler, CMake 3.12+, OpenSSL, Conan.
-   **Build steps**:
    1.  Initialize Conan dependencies using `BuildInitwithConan.bat` (Windows) or `BuildInitwithConan.sh` (Linux/macOS).
    2.  Configure and build with CMake.
-   **Run steps**: The core engine is a library designed for integration into other applications. It is not a standalone executable.
-   **Tests / CI**:
    -   CI is configured via GitHub Actions (`.github/workflows/`).
    -   Testing is executed using `ctest`.
-   **Generating Documentation:**
    -   Documentation is built using Doxygen and Sphinx.
    -   Run `doxygen ./Doxyfile` to generate XML from C++ comments.
    -   Run `sphinx-build -b html ./document_sources docs` to build the HTML site.
    -   The entire process is automated by `DOCUMENT_GENERATOR.sh`.

### 4.4 Documentation Strategy (the plan)

**Phase C: Verifying `agents-memo.md` Content (Complete)**

This phase, focused on ensuring the accuracy and completeness of the information gathered in `agents-memo.md` through spot-checks and cross-referencing, is now **complete**.

**Phase B: Populating `agents-memo.md` (Complete)**

Phase B (populating `agents-memo.md` with a comprehensive analysis of the project) is now **complete**. The `agents-memo.md` document provides a foundational understanding of the project's architecture, conventions, and purpose.

**Phase A: Initial Analysis and Strategy (Complete)**

Initial analysis of the project structure and documentation was completed, and a documentation strategy was formulated.

### 4.5 Doc Inventory (what exists today)
-   `document_sources/`: Source directory for Sphinx documentation, containing `.rst` files and `conf.py`.
-   `docs/`: Generated HTML documentation.
-   `Doxyfile`: Doxygen configuration file.
-   `DOCUMENT_GENERATOR.sh`: Script to build the documentation (Doxygen + Sphinx).
-   `README.md`: Project overview.
-   `BluePrint_PDJE/`: Design documents.
-   `.github/workflows/`: CI/CD workflows.
-   `include/core/interface/PDJE_interface.hpp`: Public C++ API with Doxygen comments.
-   `agents-memo.md`: An internal memo for recording project information and analysis. **(Now comprehensively updated and verified)**

### 4.6 Open Questions / Unknowns (Refined)
-   **P0**: What is the rationale behind placing all C++ source code (headers and implementation) within the `include/` directory? (Partially addressed by analysis, confirmed to be for build simplicity and library consumption, but full historical rationale is still open.)
-   **P1**: Are there typical integration patterns for C++ applications (beyond simple tests) *within this repository*? (Not fully addressed yet, `PDJE_Godot_Plugin` mentioned in CI, but no direct C++ integration examples found within this repo).
-   **P2**: How are the tests defined in `CMakeLists.txt` typically executed for local development? (Covered by `ctest` in CI, local execution would be similar).
-   **P3**: Clarify the intended use and implementation details of the `Utility Engine` module, as it remains a placeholder.
-   **P4**: Define the exact requirements and scope for the `AXIS_MODEL` class, which is a planned feature for handling continuous input.

### 4.7 Next Actions (1–2 hours scope)

1.  **Update `Input_Engine.rst`:** Remove the outdated warning about Windows-only support and update with details about multi-platform `DefaultDevs` implementations (Linux `libevdev`, Windows Raw Input API with IPC).

### 4.8 Change Log (keep brief)
-   2026-02-02: Initial draft of `gemini.md`.
-   2026-02-02: Updates from `README.md`, `PDJE_VERSION`, `CMakeLists.txt`, GitHub workflows, `BluePrint_PDJE`, and `PDJE_interface.hpp`.
-   2026-02-02: Major revision of the documentation plan to align with the existing Doxygen + Sphinx documentation structure.
-   2026-02-02: Identified and recorded discrepancies between existing documentation and source code.
-   2026-02-02: Created `agents-memo.md` to record project information and analysis.
-   2026-02-02: Redefined Phase B to be the process of populating `agents-memo.md`.
-   2026-02-02: Removed all SWIG-related content from `document_sources/index.rst` and `document_sources/Core_Engine.rst`, resolving the identified documentation discrepancies. Updated "Next Actions" accordingly.
-   2026-02-02: Completed reading of all `.rst` files and `conf.py` in `document_sources/`, and updated `agents-memo.md` with gathered information and identified discrepancies. Updated "Next Actions" to focus on source code analysis.
-   2026-02-03: **Phase B Complete.** `agents-memo.md` has been comprehensively updated with detailed analysis from a deep dive into all core source code.
-   2026-02-03: **Phase C Complete.** Verification of `agents-memo.md` content is complete. Next actions updated to begin actual documentation updates.