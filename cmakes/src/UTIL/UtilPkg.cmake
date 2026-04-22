if(TARGET PDJE_UTIL)
  return()
endif()

include(cmakes/src/UTIL/UTILsrc.cmake)

function(setUtilReqs targetName)
  DynamicInnerFlag(${targetName})
  setRocksDBReqLib(${targetName})
  setHighwayReqLib(${targetName})
  setOpenCLRuntimeShimReqLib(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
  SET_PROPERTIES(${targetName})
endfunction(setUtilReqs)

add_library(UTIL_INCLUDE INTERFACE)
target_compile_features(UTIL_INCLUDE INTERFACE cxx_std_20)
target_include_directories(UTIL_INCLUDE
  INTERFACE
    ${PDJE_INCLUDE_UTIL}
)
setWebpReqLib(UTIL_INCLUDE)
setSqliteReqLib(UTIL_INCLUDE)
setAnnoyReqLib(UTIL_INCLUDE)
setOpenCLCppReqLib(UTIL_INCLUDE)
setCmrcReqLib(UTIL_INCLUDE)

add_library(UTIL_OBJ OBJECT
  ${PDJE_UTIL_SOURCES}
)
target_include_directories(UTIL_OBJ PRIVATE
  ${PDJE_UTIL_PRIVATE_INCLUDE}
)
target_link_libraries(UTIL_OBJ PUBLIC UTIL_INCLUDE)
setUtilReqs(UTIL_OBJ)

set(UTIL_SRC_EXPORT
  $<TARGET_OBJECTS:UTIL_OBJ>
)

set(UTIL_SRC_ONLY_EXPORT
  $<TARGET_OBJECTS:UTIL_OBJ>
)

if(PDJE_DYNAMIC)
  add_library(PDJE_UTIL SHARED ${UTIL_SRC_EXPORT})
  set_target_properties(PDJE_UTIL PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
  )
else()
  add_library(PDJE_UTIL ${UTIL_SRC_EXPORT})
endif()

target_link_libraries(PDJE_UTIL
  PRIVATE
    UTIL_OBJ
  PUBLIC
    UTIL_INCLUDE
)
DynamicOuterFlag(PDJE_UTIL)
