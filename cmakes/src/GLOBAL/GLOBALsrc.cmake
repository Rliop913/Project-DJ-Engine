set(globalSource
${CMAKE_CURRENT_SOURCE_DIR}/include/global/DataLines/fileNameSanitizer.cpp
)


add_library(GLOBAL_OBJ OBJECT
${globalSource}
)


add_library(GLOBAL_INCLUDE INTERFACE)


DynamicInnerFlag(GLOBAL_OBJ)

target_include_directories(GLOBAL_INCLUDE INTERFACE
    ${PDJE_INCLUDE_ROOT}/include/global
    ${PDJE_INCLUDE_ROOT}/include/global/DataLines
)

if(WIN32)
  target_include_directories(GLOBAL_INCLUDE INTERFACE
    ${PDJE_INCLUDE_ROOT}/include/global/Highres_Clock/Windows
  )
  
elseif(APPLE)
  target_include_directories(GLOBAL_INCLUDE INTERFACE
    ${PDJE_INCLUDE_ROOT}/include/global/Highres_Clock/Mac
  )
else()
  target_include_directories(GLOBAL_INCLUDE INTERFACE
    ${PDJE_INCLUDE_ROOT}/include/global/Highres_Clock/Linux
  )
endif()
target_link_libraries(GLOBAL_OBJ PRIVATE GLOBAL_INCLUDE)
# setSpdlogReqLib(GLOBAL_OBJ)

set(GLOBAL_SRC_EXPORT
$<TARGET_OBJECTS:GLOBAL_OBJ>
)