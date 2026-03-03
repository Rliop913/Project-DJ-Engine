include_guard(GLOBAL)

option(PDJE_HALIDE_ENABLE "Enable Halide AOT multi-vendor build outputs" OFF)
option(PDJE_HALIDE_AUTO_LINK "Automatically link generated Halide AOT libraries into PDJE targets" ON)

set(PDJE_HALIDE_GENERATOR_TARGET "pdje_halide.generator" CACHE STRING "CMake target name for the Halide generator executable")
set(PDJE_HALIDE_GENERATOR_NAME "" CACHE STRING "Registered Halide generator name")
set(PDJE_HALIDE_GENERATOR_SOURCES "" CACHE STRING "Semicolon-separated Halide generator source files")
set(PDJE_HALIDE_TARGET_PREFIX "pdje_halide" CACHE STRING "Prefix for generated Halide AOT targets")
set(PDJE_HALIDE_AUTOSCHEDULER "Halide::Adams2019" CACHE STRING "Halide autoscheduler target to use")
set(PDJE_HALIDE_BASE_TARGETS "" CACHE STRING "Override Halide base target triples with a semicolon-separated list")
set(PDJE_HALIDE_VENDOR_FEATURES "" CACHE STRING "Override Halide vendor feature list with a semicolon-separated list")
set(PDJE_HALIDE_GENERATOR_PARAMS "" CACHE STRING "Extra PARAMS forwarded to add_halide_library")

set(PDJE_HALIDE_AOT_TARGETS "")
set(PDJE_HALIDE_RUNTIME_TARGETS "")

function(_pdje_halide_install_target_group targetListVar)
  set(_built_targets "")
  foreach(_target IN LISTS ${targetListVar})
    get_property(_is_imported TARGET ${_target} PROPERTY IMPORTED)
    if(_is_imported)
      get_target_property(_imported_location ${_target} IMPORTED_LOCATION)
      if(_imported_location)
        install(FILES "${_imported_location}" DESTINATION ${CMAKE_INSTALL_LIBDIR})
      endif()
    else()
      list(APPEND _built_targets ${_target})
    endif()
  endforeach()

  if(_built_targets)
    install(TARGETS ${_built_targets}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()
endfunction()

function(pdje_halide_attach_to_target targetName)
  if(PDJE_HALIDE_ENABLE AND PDJE_HALIDE_AUTO_LINK AND TARGET PDJE_HALIDE_AOT)
    target_link_libraries(${targetName} PUBLIC PDJE_HALIDE_AOT)
  endif()
endfunction()

function(pdje_halide_install_generated_targets)
  if(NOT PDJE_HALIDE_ENABLE)
    return()
  endif()

  if(PDJE_HALIDE_RUNTIME_TARGETS)
    _pdje_halide_install_target_group(PDJE_HALIDE_RUNTIME_TARGETS)
  endif()

  if(PDJE_HALIDE_AOT_TARGETS)
    _pdje_halide_install_target_group(PDJE_HALIDE_AOT_TARGETS)
    foreach(_target IN LISTS PDJE_HALIDE_AOT_TARGETS)
      install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${_target}.h"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pdje/halide)
    endforeach()
  endif()
endfunction()

function(_pdje_halide_default_base_targets outVar)
  if(PDJE_HALIDE_BASE_TARGETS)
    set(_base_targets ${PDJE_HALIDE_BASE_TARGETS})
  elseif(WIN32)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(ARM64|arm64|aarch64)$")
      set(_base_targets arm-64-windows)
    else()
      set(_base_targets x86-64-windows)
    endif()
  elseif(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
      set(_base_targets x86-64-osx)
    else()
      set(_base_targets arm-64-osx)
    endif()
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)$")
      set(_base_targets arm-64-linux)
    else()
      set(_base_targets x86-64-linux)
    endif()
  else()
    message(FATAL_ERROR "Unsupported platform for Halide multi-vendor deployment: ${CMAKE_SYSTEM_NAME}")
  endif()

  set(${outVar} ${_base_targets} PARENT_SCOPE)
endfunction()

function(_pdje_halide_default_vendor_features outVar)
  if(PDJE_HALIDE_VENDOR_FEATURES)
    set(_vendor_features ${PDJE_HALIDE_VENDOR_FEATURES})
  elseif(APPLE)
    set(_vendor_features metal)
  elseif(WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(_vendor_features cuda opencl)
  else()
    set(_vendor_features)
  endif()

  set(${outVar} ${_vendor_features} PARENT_SCOPE)
endfunction()

if(PDJE_HALIDE_ENABLE)
  if(CMAKE_VERSION VERSION_LESS 3.28)
    message(FATAL_ERROR "PDJE_HALIDE_ENABLE requires CMake 3.28 or newer because the installed Halide package uses the 3.28 helper API.")
  endif()

  if(NOT PDJE_HALIDE_GENERATOR_NAME)
    message(FATAL_ERROR "Set PDJE_HALIDE_GENERATOR_NAME to the HALIDE_REGISTER_GENERATOR name when PDJE_HALIDE_ENABLE=ON.")
  endif()

  if(NOT PDJE_HALIDE_GENERATOR_SOURCES)
    message(FATAL_ERROR "Set PDJE_HALIDE_GENERATOR_SOURCES to at least one Halide generator source file when PDJE_HALIDE_ENABLE=ON.")
  endif()

  set(_pdje_halide_generator_sources "")
  foreach(_source IN LISTS PDJE_HALIDE_GENERATOR_SOURCES)
    get_filename_component(_resolved_source "${_source}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")
    if(NOT EXISTS "${_resolved_source}")
      message(FATAL_ERROR "Halide generator source does not exist: ${_source}")
    endif()
    list(APPEND _pdje_halide_generator_sources "${_resolved_source}")
  endforeach()

  find_package(Halide REQUIRED)

  add_halide_generator(${PDJE_HALIDE_GENERATOR_TARGET}
    SOURCES ${_pdje_halide_generator_sources})

  _pdje_halide_default_base_targets(_pdje_halide_base_targets)
  _pdje_halide_default_vendor_features(_pdje_halide_vendor_features)

  set(_pdje_halide_autoscheduler_args "")
  if(PDJE_HALIDE_AUTOSCHEDULER)
    if(TARGET ${PDJE_HALIDE_AUTOSCHEDULER})
      list(APPEND _pdje_halide_autoscheduler_args AUTOSCHEDULER ${PDJE_HALIDE_AUTOSCHEDULER})
    else()
      message(WARNING "Requested Halide autoscheduler target was not found: ${PDJE_HALIDE_AUTOSCHEDULER}. Building without an autoscheduler.")
    endif()
  endif()

  set(_pdje_halide_common_args
    FROM ${PDJE_HALIDE_GENERATOR_TARGET}
    GENERATOR ${PDJE_HALIDE_GENERATOR_NAME}
    ${_pdje_halide_autoscheduler_args})
  if(PDJE_HALIDE_GENERATOR_PARAMS)
    list(APPEND _pdje_halide_common_args PARAMS ${PDJE_HALIDE_GENERATOR_PARAMS})
  endif()

  set(_pdje_halide_generated_targets "")
  set(_pdje_halide_runtime_targets "")

  foreach(_base_target IN LISTS _pdje_halide_base_targets)
    string(REPLACE "-" "_" _base_id "${_base_target}")
    set(_pdje_halide_shared_runtime "")

    set(_cpu_target "${PDJE_HALIDE_TARGET_PREFIX}_${_base_id}_cpu")
    add_halide_library(${_cpu_target}
      ${_pdje_halide_common_args}
      TARGETS ${_base_target})
    list(APPEND _pdje_halide_generated_targets ${_cpu_target})

    get_property(_cpu_runtime TARGET ${_cpu_target} PROPERTY Halide_LIBRARY_RUNTIME_TARGET)
    if(_cpu_runtime)
      list(APPEND _pdje_halide_runtime_targets ${_cpu_runtime})
      if(WIN32 AND NOT _pdje_halide_shared_runtime)
        set(_pdje_halide_shared_runtime "${_cpu_runtime}")
      endif()
    endif()

    foreach(_vendor_feature IN LISTS _pdje_halide_vendor_features)
      set(_vendor_target "${PDJE_HALIDE_TARGET_PREFIX}_${_base_id}_${_vendor_feature}")

      set(_vendor_args
        ${_pdje_halide_common_args}
        TARGETS ${_base_target}
        FEATURES ${_vendor_feature})

      if(WIN32 AND _pdje_halide_shared_runtime)
        list(APPEND _vendor_args USE_RUNTIME ${_pdje_halide_shared_runtime})
      endif()

      add_halide_library(${_vendor_target} ${_vendor_args})
      list(APPEND _pdje_halide_generated_targets ${_vendor_target})

      get_property(_vendor_runtime TARGET ${_vendor_target} PROPERTY Halide_LIBRARY_RUNTIME_TARGET)
      if(_vendor_runtime)
        list(APPEND _pdje_halide_runtime_targets ${_vendor_runtime})
      endif()
    endforeach()
  endforeach()

  list(REMOVE_DUPLICATES _pdje_halide_generated_targets)
  list(REMOVE_DUPLICATES _pdje_halide_runtime_targets)

  add_library(PDJE_HALIDE_AOT INTERFACE)
  target_link_libraries(PDJE_HALIDE_AOT INTERFACE ${_pdje_halide_generated_targets})
  target_include_directories(PDJE_HALIDE_AOT INTERFACE ${CMAKE_CURRENT_BINARY_DIR})

  add_custom_target(pdje_halide_aot_bundle DEPENDS ${_pdje_halide_generated_targets})

  set(PDJE_HALIDE_AOT_TARGETS ${_pdje_halide_generated_targets})
  set(PDJE_HALIDE_RUNTIME_TARGETS ${_pdje_halide_runtime_targets})

  message(STATUS "PDJE Halide AOT enabled")
  message(STATUS "PDJE Halide generator target: ${PDJE_HALIDE_GENERATOR_TARGET}")
  message(STATUS "PDJE Halide base targets: ${_pdje_halide_base_targets}")
  message(STATUS "PDJE Halide vendor features: ${_pdje_halide_vendor_features}")
endif()
