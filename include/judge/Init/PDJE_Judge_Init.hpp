#pragma once

#include "PDJE_Core_DataLine.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Judge_Init_Structs.hpp"

#include "PDJE_Rule.hpp"

#include <optional>
#include <unordered_map>
namespace PDJE_JUDGE {

/** @brief Judge module initializer holding data lines, rules, and notes. */
class PDJE_API Judge_Init {
  private:
    /** @brief Push a note into main/sub buffers when axis range is provided. */
    void
    DefaultFill(NOTE            &obj,
                const uint64_t   railid,
                const LOCAL_TIME axis1,
                const LOCAL_TIME axis2);

  public:
    Custom_Events                       lambdas;
    std::optional<PDJE_CORE_DATA_LINE>  coreline;
    std::optional<PDJE_INPUT_DATA_LINE> inputline;

    // note object
    std::optional<OBJ> note_objects;

    // rules
    std::optional<EVENT_RULE>                     ev_rule;
    std::unordered_map<INPUT_RULE, INPUT_SETTING> dev_rules;

    /** @brief Register an input device rule and its target rail/offset. */
    void
    SetInputRule(const INPUT_CONFIG &device_config);
    /** @brief Set judgment window configuration. */
    void
    SetEventRule(const EVENT_RULE &event_rule);

    /** @brief Set optional callbacks for miss/use and mouse parsing. */
    void
    SetCustomEvents(const Custom_Events &events);

    /** @brief Collect note metadata and place it on the matching rail. */
    void
    NoteObjectCollector(const std::string        noteType,
                        const uint16_t           noteDetail,
                        const std::string        firstArg,
                        const std::string        secondArg,
                        const std::string        thirdArg,
                        const unsigned long long Y_Axis,
                        const unsigned long long Y_Axis_2,
                        const uint64_t           railID);

    /** @brief Attach the core data line from PDJE core engine. */
    void
    SetCoreLine(const PDJE_CORE_DATA_LINE &coreline);
    /** @brief Attach the input data line from input engine. */
    void
    SetInputLine(const PDJE_INPUT_DATA_LINE &inputline);
};
}; // namespace PDJE_JUDGE
