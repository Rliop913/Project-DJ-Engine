set(gameplaySource
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GamePlay/PDJE_GamePlay.cpp
)

if(PDJE_DYNAMIC)
    add_library(PDJE_MODULE_GAMEPLAY SHARED ${gameplaySource})
else()
    add_library(PDJE_MODULE_GAMEPLAY STATIC ${gameplaySource})
endif()

target_include_directories(PDJE_MODULE_GAMEPLAY PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GamePlay
    ${CMAKE_CURRENT_SOURCE_DIR}/include/global
)
target_link_libraries(PDJE_MODULE_GAMEPLAY PUBLIC
    PDJE PDJE_MODULE_INPUT PDJE_MODULE_JUDGE
    CORE_INCLUDE GLOBAL_INCLUDE JUDGE_INCLUDE SQL_INCLUDE libremidi
)
# Implementation headers for the three native modules (no duplicate objects).
target_link_libraries(PDJE_MODULE_GAMEPLAY PRIVATE
    INPUT_MAIN_INCLUDE IPC_INCLUDE CRYPTO_INCLUDE
    MINIAUDIO_INCLUDE SOUNDTOUCH_INCLUDE nlohmann_json::nlohmann_json
    PDJE_LOG_RUNTIME
)
setSpdlogReqLib(PDJE_MODULE_GAMEPLAY)
setCapnpReqLib(PDJE_MODULE_GAMEPLAY)
setLibgit2ReqLib(PDJE_MODULE_GAMEPLAY)
setRocksDBReqLib(PDJE_MODULE_GAMEPLAY)
setBotanReqLib(PDJE_MODULE_GAMEPLAY)
setHighwayReqLib(PDJE_MODULE_GAMEPLAY)
LinuxSetAtomic(PDJE_MODULE_GAMEPLAY)
target_compile_features(PDJE_MODULE_GAMEPLAY PUBLIC cxx_std_20)
# Do not define PDJE_BUILDING: Core/Input/Judge are DLL imports here.
if(PDJE_DYNAMIC)
    target_compile_definitions(PDJE_MODULE_GAMEPLAY PUBLIC PDJE_GAMEPLAY_SHARED)
    target_compile_definitions(PDJE_MODULE_GAMEPLAY PRIVATE PDJE_GAMEPLAY_BUILDING)
endif()
