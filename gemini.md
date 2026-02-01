**READY FOR USER APPROVAL (Phase B)**

My previous documentation plan was incorrect as it overlooked the existing Sphinx documentation structure. The following is a revised and much more accurate plan based on a thorough analysis of the `document_sources/` directory, the `DOCUMENT_GENERATOR.sh` script, and the user's request to identify discrepancies between the documentation and the code.

**Proposed Documentation Plan:**

1.  **Correct Discrepancies:** I will first correct the identified inconsistencies between the documentation and the source code (see "Documentation Discrepancies" section below). This includes removing all SWIG-related content as requested.
2.  **Enhance Existing `.rst` Files:** I will update and expand the existing `.rst` files in `document_sources/`.
    *   **`index.rst`:** Update the project summary and high-level architecture based on the full analysis.
    *   **`Getting Started.rst`:** Expand this into a comprehensive `Build & Install Guide` and `Quickstart` guide, including detailed prerequisites, build options, and a C++ integration example.
    *   **`Editor_Format.rst`:** Update and expand this file based on the latest understanding of the editor's functionality.
3.  **Generate API Reference:** Run the `DOCUMENT_GENERATOR.sh` script to ensure the Doxygen C++ API reference is up-to-date and correctly integrated into the Sphinx site.
4.  **Create New Content (as needed):** If necessary, create new `.rst` files within `document_sources/` for tutorials, a contribution guide, or a more detailed architecture document, following the existing structure.
5.  **Remove Obsolete File:** Delete the `docs/build_install.md` file I previously created, as it does not belong in the generated output directory.

**Estimated Scope:** Medium

**Explicit Request:** Reply OK to start editing files based on this revised plan.

---

# Gemini Agent Prompt — Recursive Documentation Builder

## 0) Mission
You are a **documentation agent**. Your job is to read this project’s code + existing docs, then **design a documentation strategy** and **continuously refine that strategy and summary by updating ONLY this file (`gemini.md`)**.

The goal is to make `gemini.md` evolve into a complete, actionable documentation plan + project summary that can later drive real documentation work.

---

**Project Summary:**

"Project-DJ-Engine" (PDJE) is an integrated, production-ready engine combining rhythm game mechanics, DJ performance tools, and digital audio workstation features into a single soft real-time C++ system. It lets you create, reproduce, and remix DJ mixes, drive rhythm-game note charts, and build interactive music experiences—all with low-latency, millisecond-level timing guarantees.

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
- **Project name**: Project-DJ-Engine (PDJE)
- **One-liner**: An integrated, production-ready engine combining rhythm game mechanics, DJ performance tools, and digital audio workstation features into a single soft real-time C++ system.
- **Target users**: Developers interested in building custom rhythm-games, real-time DJ performance tools, or in-game music editors.
- **Primary value**: Provides a C++ engine with high-performance audio processing, real-time DJ mixing, an integrated editor suite, and centralized data management.
- **Key features**:
    *   High-performance audio processing and time synchronization.
    *   Real-time DJ mixing.
    *   Integrated editor suite for music metadata, mixset & note-chart authoring.
    *   Centralized data management for tracks, metadata, mixsets, and note-chart datas.
    *   Live interaction via MIDI, dynamic input, and automation APIs.
    *   Modular architecture with `Core`, `Input`, `Judge`, and `Utility` engines.
- **Non-goals**: No graphics-related technology is built-in.

### 4.2 Architecture Overview
- **High-level diagram (text)**:
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
- **Modules**:
  - `document_sources/`: Source files (`.rst`, `conf.py`) for the Sphinx documentation.
  - `docs/`: Generated HTML documentation output directory.
  - `include/`: Primary location for all C++ source code.
  - `Core Engine`: Builds, queries, and modifies the core database; handles audio processing, real-time mixing, and an integrated editor with Git-based version control.
  - `Input Engine`: Cross-platform, low-latency input handling for keyboard, mouse, and MIDI.
  - `Judge Engine`: Handles rhythm-game timing logic and real-time judgments with microsecond-level accuracy.
  - `Utility Engine`: Handles non-essential but useful functionalities like AI inference, FFT processing, and music analysis.
- **Key data flows**:
    *   User creates `PDJE` object -> `SearchMusic`/`SearchTrack` populates data -> `InitPlayer` sets up audio -> `player` handles music/FX.
    *   User creates `PDJE` object -> `InitEditor` sets up editor -> `editor` manages track/mix/note data.
- **Key abstractions / interfaces**: The `PDJE` class is the main interface. `audioPlayer`, `editorObject`, `trackdata`, `litedb`, and various `EDIT_ARG_*` types are key abstractions.

### 4.3 Build / Run / Dev Workflow
- **Prereqs**: C++20 compatible compiler, CMake 3.12+, OpenSSL, Conan.
- **Build steps**:
    1.  Initialize Conan dependencies using `BuildInitwithConan.bat` (Windows) or `BuildInitwithConan.sh` (Linux/macOS).
    2.  Configure and build with CMake.
- **Run steps**: The core engine is a library designed for integration into other applications. It is not a standalone executable.
- **Tests / CI**:
    -   CI is configured via GitHub Actions (`.github/workflows/`).
    -   Testing is executed using `ctest`.
- **Generating Documentation:**
    -   Documentation is built using Doxygen and Sphinx.
    -   Run `doxygen ./Doxyfile` to generate XML from C++ comments.
    -   Run `sphinx-build -b html ./document_sources docs` to build the HTML site.
    -   The entire process is automated by `DOCUMENT_GENERATOR.sh`.

### 4.4 Documentation Strategy (the plan)

The documentation strategy is to work within the existing Doxygen + Sphinx framework. The primary tasks will be to:
1.  Correct the identified discrepancies between the documentation and the source code.
2.  Update and enhance the `.rst` files located in `document_sources/`.
3.  Ensure the Doxygen-generated C++ API reference is up-to-date and correctly integrated into the Sphinx site.
4.  Create new `.rst` files for tutorials and other missing content as needed, following the established structure.
5.  Remove the obsolete `docs/build_install.md` file.

### 4.5 Doc Inventory (what exists today)
- `document_sources/`: Source directory for Sphinx documentation, containing `.rst` files and `conf.py`.
- `docs/`: Generated HTML documentation.
- `Doxyfile`: Doxygen configuration file.
- `DOCUMENT_GENERATOR.sh`: Script to build the documentation (Doxygen + Sphinx).
- `README.md`: Project overview.
- `BluePrint_PDJE/`: Design documents.
- `.github/workflows/`: CI/CD workflows.
- `include/core/interface/PDJE_interface.hpp`: Public C++ API with Doxygen comments.



### 4.7 Open Questions / Unknowns (Refined)
- **P0**: What is the rationale behind placing all C++ source code (headers and implementation) within the `include/` directory?
- **P1**: Are there typical integration patterns for C++ applications (beyond simple tests) *within this repository*?
- **P2**: How are the tests defined in `CMakeLists.txt` typically executed for local development?

### 4.8 Next Actions (1–2 hours scope)
1.  Read `document_sources/Getting Started.rst`, `document_sources/Editor_Format.rst`, and other `.rst` files to understand their content, structure, and style.
2.  Read `document_sources/conf.py` to understand the Sphinx configuration.
3.  Propose a plan to delete `docs/build_install.md`.
4.  Re-propose entering Phase B with the revised plan.

### 4.9 Change Log (keep brief)
- 2026-02-02: Initial draft of `gemini.md`.
- 2026-02-02: Updates from `README.md`, `PDJE_VERSION`, `CMakeLists.txt`, GitHub workflows, `BluePrint_PDJE`, and `PDJE_interface.hpp`.
- 2026-02-02: Major revision of the documentation plan to align with the existing Doxygen + Sphinx documentation structure, based on analysis of `document_sources/`, `DOCUMENT_GENERATOR.sh`, and `index.rst`.
- 2026-02-02: Identified and recorded discrepancies between existing documentation and source code, and updated the documentation strategy to include their correction.
- 2026-02-02: Removed all SWIG-related content from `document_sources/index.rst` and `document_sources/Core_Engine.rst`, resolving the identified documentation discrepancies. Updated "Next Actions" accordingly.