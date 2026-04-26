include_guard(GLOBAL)

set(PDJE_ONNXRUNTIME_VERSION "1.25.0" CACHE INTERNAL "Pinned ONNX Runtime prebuilt version")
set(_pdje_onnxruntime_supported_matrix "Windows x64, Linux x64, macOS arm64")
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _pdje_onnxruntime_processor_lower)

function(_pdje_onnxruntime_unsupported)
  message(
    FATAL_ERROR
      "PDJE ONNX Runtime ${PDJE_ONNXRUNTIME_VERSION} prebuilt only supports "
      "${_pdje_onnxruntime_supported_matrix}. "
      "Detected CMAKE_SYSTEM_NAME='${CMAKE_SYSTEM_NAME}', "
      "CMAKE_SYSTEM_PROCESSOR='${CMAKE_SYSTEM_PROCESSOR}'.")
endfunction()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if(_pdje_onnxruntime_processor_lower MATCHES "^(amd64|x86_64)$")
    set(_pdje_onnxruntime_archive_name "onnxruntime-win-x64-1.25.0.zip")
    set(_pdje_onnxruntime_archive_hash "DA753F762BF2400E7191EC594086B186A7051D5AF8DC886F6E2020C2403DF738")
    set(_pdje_onnxruntime_library_suffix "lib/onnxruntime.lib")
    set(_pdje_onnxruntime_runtime_suffix "lib/onnxruntime.dll")
    set(_pdje_onnxruntime_runtime_suffixes
      "lib/onnxruntime.dll"
      "lib/onnxruntime_providers_shared.dll")
  else()
    _pdje_onnxruntime_unsupported()
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  if(_pdje_onnxruntime_processor_lower MATCHES "^(amd64|x86_64)$")
    set(_pdje_onnxruntime_archive_name "onnxruntime-linux-x64-1.25.0.tgz")
    set(_pdje_onnxruntime_archive_hash "E0A8998E70416801F9A634A8EA1D369A255FF109741469F9D99CF369A46A1492")
    set(_pdje_onnxruntime_runtime_suffix "lib/libonnxruntime.so")
    set(_pdje_onnxruntime_runtime_suffixes
      "lib/libonnxruntime.so"
      "lib/libonnxruntime.so.1"
      "lib/libonnxruntime.so.1.25.0"
      "lib/libonnxruntime_providers_shared.so")
  else()
    _pdje_onnxruntime_unsupported()
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(_pdje_onnxruntime_processor_lower MATCHES "^(arm64|aarch64)$")
    set(_pdje_onnxruntime_archive_name "onnxruntime-osx-arm64-1.25.0.tgz")
    set(_pdje_onnxruntime_archive_hash "65405DC8793C86CADB98B5E07F6D3BDDE84F8300F1B030D4736B41C17610D6C1")
    set(_pdje_onnxruntime_runtime_suffix "lib/libonnxruntime.dylib")
    set(_pdje_onnxruntime_runtime_suffixes
      "lib/libonnxruntime.dylib"
      "lib/libonnxruntime.1.25.0.dylib")
  else()
    _pdje_onnxruntime_unsupported()
  endif()
else()
  _pdje_onnxruntime_unsupported()
endif()

set(_pdje_onnxruntime_archive_url
    "https://github.com/microsoft/onnxruntime/releases/download/v${PDJE_ONNXRUNTIME_VERSION}/${_pdje_onnxruntime_archive_name}")
get_filename_component(_pdje_onnxruntime_archive_root "${_pdje_onnxruntime_archive_name}" NAME_WE)

FetchContent_Declare(
  pdje_onnxruntime_prebuilt
  URL "${_pdje_onnxruntime_archive_url}"
  URL_HASH "SHA256=${_pdje_onnxruntime_archive_hash}"
)

FetchContent_GetProperties(pdje_onnxruntime_prebuilt)
if(NOT pdje_onnxruntime_prebuilt_POPULATED)
  FetchContent_Populate(pdje_onnxruntime_prebuilt)
endif()

set(_pdje_onnxruntime_package_dir "${pdje_onnxruntime_prebuilt_SOURCE_DIR}")
if(NOT EXISTS "${_pdje_onnxruntime_package_dir}/include/onnxruntime_c_api.h")
  set(_pdje_onnxruntime_package_dir
      "${pdje_onnxruntime_prebuilt_SOURCE_DIR}/${_pdje_onnxruntime_archive_root}")
endif()

if(NOT EXISTS "${_pdje_onnxruntime_package_dir}/include/onnxruntime_c_api.h")
  message(
    FATAL_ERROR
      "PDJE could not locate ONNX Runtime headers after extracting "
      "${_pdje_onnxruntime_archive_name}. Expected to find "
      "'${_pdje_onnxruntime_package_dir}/include/onnxruntime_c_api.h'.")
endif()

set(_pdje_onnxruntime_runtime_path
    "${_pdje_onnxruntime_package_dir}/${_pdje_onnxruntime_runtime_suffix}")
if(NOT EXISTS "${_pdje_onnxruntime_runtime_path}")
  message(
    FATAL_ERROR
      "PDJE could not locate ONNX Runtime runtime library at "
      "'${_pdje_onnxruntime_runtime_path}'.")
endif()

if(WIN32)
  set(_pdje_onnxruntime_import_library_path
      "${_pdje_onnxruntime_package_dir}/${_pdje_onnxruntime_library_suffix}")
  if(NOT EXISTS "${_pdje_onnxruntime_import_library_path}")
    message(
      FATAL_ERROR
        "PDJE could not locate ONNX Runtime import library at "
        "'${_pdje_onnxruntime_import_library_path}'.")
  endif()
endif()

set(_pdje_onnxruntime_runtime_files "")
foreach(_pdje_onnxruntime_runtime_suffix IN LISTS _pdje_onnxruntime_runtime_suffixes)
  set(_pdje_onnxruntime_runtime_file
      "${_pdje_onnxruntime_package_dir}/${_pdje_onnxruntime_runtime_suffix}")
  if(NOT EXISTS "${_pdje_onnxruntime_runtime_file}")
    message(
      FATAL_ERROR
        "PDJE expected ONNX Runtime runtime artifact "
        "'${_pdje_onnxruntime_runtime_file}' but it was not present.")
  endif()
  list(APPEND _pdje_onnxruntime_runtime_files "${_pdje_onnxruntime_runtime_file}")
endforeach()

if(NOT TARGET onnxruntime::onnxruntime)
  add_library(onnxruntime::onnxruntime SHARED IMPORTED GLOBAL)
endif()

if(WIN32)
  set_target_properties(
    onnxruntime::onnxruntime
    PROPERTIES
      IMPORTED_IMPLIB "${_pdje_onnxruntime_import_library_path}"
      IMPORTED_LOCATION "${_pdje_onnxruntime_runtime_path}"
      INTERFACE_INCLUDE_DIRECTORIES "${_pdje_onnxruntime_package_dir}/include")
else()
  set_target_properties(
    onnxruntime::onnxruntime
    PROPERTIES
      IMPORTED_LOCATION "${_pdje_onnxruntime_runtime_path}"
      INTERFACE_INCLUDE_DIRECTORIES "${_pdje_onnxruntime_package_dir}/include")
endif()

set(PDJE_ONNXRUNTIME_PACKAGE_DIR
    "${_pdje_onnxruntime_package_dir}"
    CACHE INTERNAL
    "Resolved ONNX Runtime prebuilt root directory"
    FORCE)
set(PDJE_ONNXRUNTIME_RUNTIME_FILES
    "${_pdje_onnxruntime_runtime_files}"
    CACHE INTERNAL
    "ONNX Runtime runtime files copied next to smoke and probe binaries"
    FORCE)

function(setOnnxRuntimeReqLib targetName)
  set(_pdje_requested_scope "")
  if(ARGC GREATER 1)
    set(_pdje_requested_scope "${ARGV1}")
  endif()

  if(NOT TARGET onnxruntime::onnxruntime)
    message(FATAL_ERROR "onnxruntime::onnxruntime target is unavailable")
  endif()

  pdje_resolve_dependency_scope(
    _pdje_onnxruntime_scope
    ${targetName}
    PRIVATE
    "${_pdje_requested_scope}")
  target_link_libraries(${targetName} ${_pdje_onnxruntime_scope} onnxruntime::onnxruntime)
endfunction()

function(pdje_copy_onnxruntime_runtime targetName)
  if("${PDJE_ONNXRUNTIME_RUNTIME_FILES}" STREQUAL "")
    return()
  endif()

  add_custom_command(
    TARGET ${targetName}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PDJE_ONNXRUNTIME_RUNTIME_FILES}
            $<TARGET_FILE_DIR:${targetName}>
    COMMAND_EXPAND_LISTS
    VERBATIM)
endfunction()
