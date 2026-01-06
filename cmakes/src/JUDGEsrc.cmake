set(judgeSource
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/PDJE_Judge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/NoteOBJ/PDJE_Note_OBJ.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Init/PDJE_Judge_Init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/PDJE_Judge_Loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/Keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/Mouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/MIDI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/PDJE_Match.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/PreProcess/PDJE_PreProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/PreProcess/PDJE_PreProcess_Work.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/InputParser/InputParser.cpp
    
    
    ${CAPNP_SRCS}
)