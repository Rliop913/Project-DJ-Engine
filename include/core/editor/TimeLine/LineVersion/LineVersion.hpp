#pragma once

#include "EventMarker.hpp"
namespace PDJE_TIMELINE{
    class LineVersion{
        private:
        
        public:
        bool Diverge();
        void ListLines();
        bool MoveLine();
        LineVersion();
        ~LineVersion();
    };
};