
set(SWIG_USE_DEBUG_PYTHON OFF)
set(Python_FIND_DEBUG FALSE)
add_definitions(-DSWIG_PYTHON_INTERPRETER_NO_DEBUG)
set(SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

set_source_files_properties(PDJE_swig.i PROPERTIES CPLUSPLUS ON)

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/swig_csharp)
swig_add_library(pdje_csharp
TYPE MODULE
LANGUAGE CSharp
OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/swig_csharp
SOURCES PDJE_swig.i ${miniaudio_src}  ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src} ${editorSource}
)




find_package(Python REQUIRED COMPONENTS Interpreter Development)

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/swig_python)
swig_add_library(pdje_python
TYPE MODULE
LANGUAGE Python
OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/swig_python
SOURCES PDJE_swig.i ${miniaudio_src}  ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src} ${editorSource}
)

set_target_properties(${SWIG_MODULE_pdje_python_REAL_NAME} PROPERTIES
  OUTPUT_NAME "_pdje_python"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/swig_python
  MAP_IMPORTED_CONFIG_DEBUG Release
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  SWIG_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/swig_gen/python
)



set_target_properties(${SWIG_MODULE_pdje_csharp_REAL_NAME} PROPERTIES
  OUTPUT_NAME "libpdje_csharp"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/swig_csharp
  MAP_IMPORTED_CONFIG_DEBUG Release
  SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON
  SWIG_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/swig_gen/csharp
)

target_include_directories(${SWIG_MODULE_pdje_python_REAL_NAME}
  PRIVATE ${Python_INCLUDE_DIRS}
)

target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE nlohmann_json::nlohmann_json rocksdb Annoy::Annoy Python::Python)
target_include_directories(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE ${PDJE_INCLUDE_CORE})

setSpdlogReqLib(${SWIG_MODULE_pdje_python_REAL_NAME})
setCapnpReqLib(${SWIG_MODULE_pdje_python_REAL_NAME})
setLibgit2ReqLib(${SWIG_MODULE_pdje_python_REAL_NAME})
setHighwayReqLib(${SWIG_MODULE_pdje_python_REAL_NAME})

target_link_libraries(${SWIG_MODULE_pdje_csharp_REAL_NAME} PRIVATE nlohmann_json::nlohmann_json rocksdb Annoy::Annoy)
target_include_directories(${SWIG_MODULE_pdje_csharp_REAL_NAME} PRIVATE ${PDJE_INCLUDE_CORE})
setSpdlogReqLib(${SWIG_MODULE_pdje_csharp_REAL_NAME})
setCapnpReqLib(${SWIG_MODULE_pdje_csharp_REAL_NAME})
setLibgit2ReqLib(${SWIG_MODULE_pdje_csharp_REAL_NAME})
setHighwayReqLib(${SWIG_MODULE_pdje_csharp_REAL_NAME})
