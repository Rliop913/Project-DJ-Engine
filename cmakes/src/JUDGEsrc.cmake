set(judgeSource
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/PDJE_Judge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/NoteOBJ/PDJE_Note_OBJ.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Init/PDJE_Judge_Init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/PDJE_Judge_Loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Mouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/MIDI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/HID.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/InputParser/InputParser.cpp
    
    
    ${CAPNP_SRCS}
)