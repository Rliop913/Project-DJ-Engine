# set(CMAKE_SWIG_FLAGS "-I/usr/share/swig/4.3.0" "-cpperraswarn")

swig_add_library(pdje_csharp
  TYPE MODULE
  LANGUAGE CSharp
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_csharp
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src}
)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

swig_add_library(pdje_python
  TYPE MODULE
  LANGUAGE Python
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_python
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src}
)

swig_add_library(pdje_go
  TYPE STATIC
  LANGUAGE go
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_go
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src}
)

set_target_properties(${SWIG_MODULE_pdje_python_REAL_NAME} PROPERTIES
  OUTPUT_NAME "_pdje_python"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/swig_python
)



set_target_properties(${SWIG_MODULE_pdje_csharp_REAL_NAME} PROPERTIES
  OUTPUT_NAME "libpdje_csharp"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/swig_csharp
)
target_include_directories(${SWIG_MODULE_pdje_python_REAL_NAME}
  PRIVATE ${Python_INCLUDE_DIRS}
)

target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE Python::Python)

target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE hwy)
target_link_libraries(${SWIG_MODULE_pdje_csharp_REAL_NAME} PRIVATE hwy)
