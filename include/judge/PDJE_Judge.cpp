#include "PDJE_Judge.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <atomic>
#include <exception>
#include <thread>
#include <unordered_map>
namespace PDJE_JUDGE {

JUDGE::JUDGE()
{
}

JUDGE_STATUS
JUDGE::Start()
{
    startlog();
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
    if (inits.raildb.Empty()) {
        warnlog("failed to start pdje judge module. no input device added. you "
                "should connect input device. check SetInputRule function.");
        return JUDGE_STATUS::INPUT_RULE_IS_EMPTY;
    }
    inits.note_objects->Sort();

    loop_obj.emplace(inits);
    loop_obj->loop_switch = true;
    loop_obj->StartEventLoop();
    loop.emplace([this]() {
        try {
            loop_obj->loop();
        } catch (const std::exception &e) {
            critlog("loop has exceptions. What: ");
            critlog(e.what());
        }
    });
    return JUDGE_STATUS::OK;
}

void
JUDGE::End()
{
    if (loop_obj.has_value()) {
        loop_obj->loop_switch = false;
        loop_obj->EndEventLoop();
    }
    if (loop.has_value() && loop->joinable()) {
        loop->join();
    }
    loop.reset();
    loop_obj.reset();
    inits.coreline.reset();
    inits.inputline.reset();
    inits.note_objects.reset();
    inits.ev_rule.reset();
    inits.raildb.Clear();
}

}; // namespace PDJE_JUDGE
