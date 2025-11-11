set(globalSource
${CMAKE_CURRENT_SOURCE_DIR}/include/global/DataLines/fileNameSanitizer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/PDJE_Crypto_Token.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/PDJE_Crypto_Hash.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/PDJE_Crypto_AEAD.cpp

)

if(WIN32)
list(APPEND globalSource
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/Windows/PSKPipe.cpp
)
elseif(APPLE)
list(APPEND globalSource
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/Mac/PSKPipe.cpp
)
else()
list(APPEND globalSource
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global/Crypto/Linux/PSKPipe.cpp
)
endif()