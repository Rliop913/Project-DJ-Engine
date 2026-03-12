set(globalSource
${CMAKE_CURRENT_SOURCE_DIR}/include/global/DataLines/fileNameSanitizer.cpp
)


add_library(GLOBAL_OBJ OBJECT
${globalSource}
)

target_include_directories(GLOBAL_OBJ PUBLIC
${PDJE_INCLUDE_GLOBAL}
)
setSpdlogReqLib(GLOBAL_OBJ)

set(GLOBAL_SRC_EXPORT
$<TARGET_OBJECTS:GLOBAL_OBJ>
)