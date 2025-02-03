#include "RenderThreads.hpp"


std::optional<JOB>
JobQueue::pop()
{
    {
        std::lock_guard<std::mutex> lock(Q_lock);
        if(jobs.empty()){
            return std::nullopt;
        }
        else{
            auto obj = std::move(jobs.front());
            jobs.pop_front();
            return std::move(obj);
        }
    }
}

int
JobQueue::push(WORK& newjob)
{
    {
        std::lock_guard<std::mutex> lock(Q_lock);
        //Impl Later.
        // jobs.push_back(newjob)
    }
}