
option(PDJE_TEST "Enable unit tests (doctest + CTest)" ON)
option(PDJE_DEV_TEST "Enable legacy/manual dev test executables" OFF)

option(PDJE_BENCHMARK "Enable benchmark codes" OFF)

option(PDJE_SWIG_BUILD "Enable build with multiple languages" OFF)
option(PDJE_DYNAMIC "Make Shared Library" OFF)

option(PDJE_RTTI_FORCE_OFF "turn off frtti option with force" OFF)

if(NOT APPLE)
# if(WIN32)
  option(PDJE_DEVELOP_INPUT "Enable linux develop build" ON)
else()
  option(PDJE_DEVELOP_INPUT "Enable linux develop build" OFF)
endif()

if(APPLE AND PDJE_DEVELOP_INPUT)
  message(WARNING "PDJE_DEVELOP_INPUT is unsupported on macOS. Forcing OFF.")
  set(PDJE_DEVELOP_INPUT OFF CACHE BOOL "Enable linux develop build" FORCE)
endif()

option(PDJE_LOG_STRICT_EXPLICIT_INIT "Disable lazy logging auto-init and require explicit pdje_logging_init_v1/startlog" OFF)
