#include "PDJE_Judge.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <atomic>
#include <thread>
#include <unordered_map>
namespace PDJE_JUDGE {

JUDGE::JUDGE()
{
    startlog();
}

JUDGE_STATUS
JUDGE::Start()
{
    if (!inits.coreline.has_value()) {
        warnlog("failed to start pdje judge module. core line is missing. "
                "please connect core data line.");
        return JUDGE_STATUS::CORE_LINE_IS_MISSING;
    }
    if (!inits.inputline.has_value()) {
        warnlog("failed to start pdje judge module. input line is missing. "
                "please connext input data line.");
        return JUDGE_STATUS::INPUT_LINE_IS_MISSING;
    }
    if (!inits.note_objects.has_value()) {
        warnlog("failed to start pdje judge module. could't find any note "
                "objects. please check Note object collector again.");
        return JUDGE_STATUS::NOTE_OBJECT_IS_MISSING;
    }
    if (!inits.ev_rule.has_value()) {
        warnlog("failed to start pdje judge module. event rule is empty. you "
                "should add miss/use range. the range is microsecond. better "
                "check again.");
        return JUDGE_STATUS::EVENT_RULE_IS_EMPTY;
    }
    if (inits.dev_rules.empty()) {
        warnlog("failed to start pdje judge module. no input device added. you "
                "should connect input device. check SetInputRule function.");
        return JUDGE_STATUS::INPUT_RULE_IS_EMPTY;
    }
    inits.note_objects->Sort();

    loop.emplace([this]() {
        loop_obj.emplace(inits);
        loop_obj->loop_switch = true;
        loop_obj->StartEventLoop();
        loop_obj->loop();
    });
    return JUDGE_STATUS::OK;
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