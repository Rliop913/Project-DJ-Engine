#pragma once

#include "EventMarker.hpp"
namespace PDJE_TIMELINE{
    class LineVersion{
        private:
            std::shared_ptr<GitData> git;
        public:
        bool Diverge();//todo-impl
        void ListLines();//todo-impl
        bool MoveLine();//todo-impl
        LineVersion(const std::shared_ptr<GitData>& gitptr)
        :git(gitptr){}
        ~LineVersion();
    };
};