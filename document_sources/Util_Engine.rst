Util_Engine
=====================

Status
------

The Utility Engine is currently a roadmap area, not an active module target in the current codebase.

- There is no `PDJE_MODULE_UTILITY` target in `CMakeLists.txt`.
- Existing build targets focus on Core (`PDJE`), Input (`PDJE_MODULE_INPUT`), and Judge (`PDJE_MODULE_JUDGE`) depending on options.

How to read this page
---------------------

Treat Utility Engine references as design direction for future releases (for example, analysis/AI helper modules), not as immediately available runtime APIs.

If you need production functionality today, use:

- :doc:`Core_Engine` for playback and editor workflows
- :doc:`Input_Engine` for keyboard/mouse/MIDI ingestion
- :doc:`Judge_Engine` for timing judgment and note matching
