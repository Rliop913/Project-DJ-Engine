#pragma once

#include <optional>

#include "PDJE_Core_DataLine.hpp"
#include "PDJE_Input_DataLine.hpp"
#include <vector>
enum JUDGE_STATUS{
    OK = 0,
    CORE_LINE_IS_MISSING = 1,
    INPUT_LINE_IS_MISSING,
    NOTE_OBJECT_IS_MISSING,
};




class PDJE_JUDGE{
private:
    
    std::optional<PDJE_CORE_DATA_LINE> core;
    std::optional<PDJE_INPUT_DATA_LINE> input;
    bool gotCoreLine = false;
    bool gotInputLine = false;
    bool gotNoteObjects = false;
public:
    void NoteObjectCollector(const std::string noteType,
    const std::string noteDetail,
    const std::string firstArg,
    const std::string secondArg,
    const std::string thirdArg,
    const unsigned long long Y_Axis,
    const unsigned long long Y_Axis_2);
    
    void SetCoreLine(const PDJE_CORE_DATA_LINE& coreline);
    void SetInputLine(const PDJE_INPUT_DATA_LINE& inputline);
    
    JUDGE_STATUS Start();
    JUDGE_STATUS CheckStatus();
    PDJE_JUDGE() = default;
    ~PDJE_JUDGE() = default;
};