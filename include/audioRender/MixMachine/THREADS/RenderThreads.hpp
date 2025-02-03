#pragma once

#include <functional>
#include <deque>
#include <mutex>
#include <optional>

using WORK = std::function<void()>;

struct JOB{
    WORK threadFunction;
    JOB* nextFunction;
};

struct JobQueue{
    std::deque<JOB> jobs;
    std::mutex Q_lock;
    std::optional<JOB> pop();
    int push(WORK& newJob);
};// Impl later.

