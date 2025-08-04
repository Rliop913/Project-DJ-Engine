# set(CMAKE_SWIG_FLAGS "-I/usr/share/swig/4.3.0" "-cpperraswarn")

swig_add_library(pdje_csharp
  TYPE MODULE
  LANGUAGE CSharp
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/swig_csharp
  SOURCES PDJE_swig.i ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource} ${sql_amalgam_src} ${editorSource} ${nanolog_src}
)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

swig_add_library(pdje_python
  TYPE MODULE
  LANGUAGE Python
  OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/swig_python
  SOURCES PDJE_swig.i ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource} ${sql_amalgam_src} ${editorSource} ${nanolog_src}
)

# swig_add_library(pdje_go
#   TYPE STATIC
#   LANGUAGE go
#   OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/swig_go
#   SOURCES PDJE_swig.i ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource} ${sql_amalgam_src} ${editorSource} ${nanolog_src}
# )

set_target_properties(${SWIG_MODULE_pdje_python_REAL_NAME} PROPERTIES
  OUTPUT_NAME "_pdje_python"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/swig_python
)



set_target_properties(${SWIG_MODULE_pdje_csharp_REAL_NAME} PROPERTIES
  OUTPUT_NAME "libpdje_csharp"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/swig_csharp
)
target_include_directories(${SWIG_MODULE_pdje_python_REAL_NAME}
  PRIVATE ${Python_INCLUDE_DIRS}
)

target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE nlohmann_json::nlohmann_json Python::Python rocksdb Annoy)

# target_link_libraries(${SWIG_MODULE_pdje_go_REAL_NAME} PRIVATE nlohmann_json::nlohmann_json hwy rocksdb Annoy)
target_link_libraries(${SWIG_MODULE_pdje_csharp_REAL_NAME} PRIVATE nlohmann_json::nlohmann_json hwy rocksdb Annoy)

if(WIN32)
target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32)
# target_link_libraries(${SWIG_MODULE_pdje_go_REAL_NAME} PRIVATE libgit2_static winhttp crypt32 rpcrt4 )
target_link_libraries(${SWIG_MODULE_pdje_csharp_REAL_NAME} PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32)
add_dependencies(${SWIG_MODULE_pdje_python_REAL_NAME}  libgit2_static)
# add_dependencies(${SWIG_MODULE_pdje_go_REAL_NAME}  libgit2_static)
add_dependencies(${SWIG_MODULE_pdje_csharp_REAL_NAME}  libgit2_static)
# target_compile_definitions(${SWIG_MODULE_pdje_python_REAL_NAME} PUBLIC PDJE_WINDOWS_DLL)
# target_compile_definitions(${SWIG_MODULE_pdje_csharp_REAL_NAME} PUBLIC PDJE_WINDOWS_DLL)
# target_compile_definitions(PDJE_dynamic PRIVATE PDJE_BUILDING)
endif()