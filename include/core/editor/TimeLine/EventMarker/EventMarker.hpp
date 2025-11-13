#pragma once
#include "GitDatas.hpp"

namespace PDJE_TIMELINE{
    class EventMarker{
        private:
        bool isActivate();
        public:
        GitData GD;
        bool Append();
        bool MoveFront();
        bool MoveBack();
        bool Move();
        bool Activate();
        EventMarker();
        ~EventMarker();
    };
};