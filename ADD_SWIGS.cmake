
swig_add_library(pdje_csharp
  TYPE STATIC
  LANGUAGE CSharp
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_csharp
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource}
)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

swig_add_library(pdje_python
  TYPE STATIC
  LANGUAGE Python
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_python
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource}
)

swig_add_library(pdje_go
  TYPE STATIC
  LANGUAGE go
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_go
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource}
)

target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE Python::Python)
