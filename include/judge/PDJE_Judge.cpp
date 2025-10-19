#include "PDJE_Judge.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <atomic>
#include <thread>
#include <unordered_map>
namespace PDJE_JUDGE {

JUDGE_STATUS
JUDGE::Start()
{
    if (!inits.coreline.has_value()) {
        return JUDGE_STATUS::CORE_LINE_IS_MISSING;
    }
    if (!inits.inputline.has_value()) {
        return JUDGE_STATUS::INPUT_LINE_IS_MISSING;
    }
    if (!inits.note_objects.has_value()) {
        return JUDGE_STATUS::NOTE_OBJECT_IS_MISSING;
    }
    if (!inits.ev_rule.has_value()) {
        return JUDGE_STATUS::EVENT_RULE_IS_EMPTY;
    }
    if (inits.dev_rules.empty()) {
        return JUDGE_STATUS::INPUT_RULE_IS_EMPTY;
    }
    inits.note_objects->Sort();

    loop_obj->StartEventLoop();

    loop.emplace([this]() {
        loop_obj.emplace(inits);
        loop_obj->loop_switch = true;
        loop_obj->loop();
    });
    return JUDGE_STATUS::OK;
}

JUDGE_STATUS
JUDGE::CheckStatus()
{
    return status;
}

void
JUDGE::End()
{
    loop_obj->loop_switch = false;
    loop_obj->EndEventLoop();
    loop->join();
    inits.coreline.reset();
    inits.inputline.reset();
    inits.note_objects.reset();
    inits.ev_rule.reset();
    inits.dev_rules.clear();
}

}; // namespace PDJE_JUDGE