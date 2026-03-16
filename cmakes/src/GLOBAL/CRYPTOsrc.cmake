set(cryptoSource
${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/PDJE_Crypto_Token.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/PDJE_Crypto_Hash.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/PDJE_Crypto_AEAD.cpp

)

if(WIN32)
list(APPEND cryptoSource
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/Windows/PSKPipe.cpp
)
elseif(APPLE)
list(APPEND cryptoSource
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/Mac/PSKPipe.cpp
)
else()
list(APPEND cryptoSource
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/Linux/PSKPipe.cpp
)
endif()

# set(PDJE_INCLUDE_CRYPTO
# ${PDJE_INCLUDE_GLOBAL}
# ${PDJE_INCLUDE_ROOT}/include/global/Crypto
# )

add_library(CRYPTO_OBJ OBJECT
${cryptoSource}
)


add_library(CRYPTO_INCLUDE INTERFACE)
target_include_directories(CRYPTO_INCLUDE INTERFACE 

${PDJE_INCLUDE_ROOT}/include/global/Crypto
)



# target_include_directories(CRYPTO_OBJ PRIVATE CRY)
target_link_libraries(CRYPTO_OBJ PUBLIC PDJE_LOG_RUNTIME CRYPTO_INCLUDE GLOBAL_INCLUDE)

setBotanReqLib(CRYPTO_OBJ)
setSpdlogReqLib(CRYPTO_OBJ)
set(CRYPTO_SRC_EXPORT
$<TARGET_OBJECTS:CRYPTO_OBJ>
)