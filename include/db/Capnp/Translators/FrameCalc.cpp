#include "FrameCalc.hpp"

namespace FrameCalc{
    unsigned long CountFrame(
        unsigned long Sbar,
        unsigned long Sbeat,
        unsigned long Sseparate,
        unsigned long Ebar,
        unsigned long Ebeat,
        unsigned long Eseparate,
        double bpm
    )
    {
        Sseparate = Sseparate > 0 ? Sseparate : 1;
        Eseparate = Eseparate > 0 ? Eseparate : 1;
        bpm = bpm > 0 ? bpm : 1;
        auto Sapprx = APPRX(double, Sbar, Sbeat, Sseparate);
        auto Eapprx = APPRX(double, Ebar, Ebeat, Eseparate);
        return static_cast<unsigned long>(
            std::round(
                (Eapprx - Sapprx) * (DMINUTE / bpm) * DSAMPLERATE
            )
        );
    }
}

bool
sortLambda(const BpmFragment& first, const BpmFragment& second)
{
    auto F = 
    static_cast<double>(first.bar) + (
        static_cast<double>(first.beat) /
        static_cast<double>(first.separate)
    );
    auto S = 
    static_cast<double>(second.bar) + (
        static_cast<double>(second.beat) /
        static_cast<double>(second.separate)
    );
    return F < S;
}


void
BpmStruct::sortFragment()
{
    if(fragments.size() > 1){
        std::sort(fragments.begin(), fragments.end(), sortLambda);
    }
}

bool
BpmStruct::calcFrame(unsigned long long StartPos)
{
    if(fragments.size() > 0){
        for(auto i : fragments){
            if(i.bpm <= 0){
                return false;
            }
        }
        fragments[0].frame_to_here = StartPos;
        if(fragments.size() > 1){
            auto Sp = &(fragments[0]);
            auto Ep = &(fragments[1]);
            for(unsigned long i=1; i<fragments.size(); ++i){
                Ep->frame_to_here =
                Sp->frame_to_here +
                FrameCalc::CountFrame(
                    Sp->bar, Sp->beat, Sp->separate,
                    Ep->bar, Ep->beat, Ep->separate,
                    Sp->bpm
                );
                ++Sp;
                ++Ep;
            }
        }
        fragments[0].frame_to_here = 0;
        return true;
    }
    else{
        return false;
    }
}

bool
searchLambda(const BpmFragment& first, const BpmFragment& second)
{
    double FA = APPRX(
        double, 
        first.bar,
        first.beat,
        first.separate);
    double SA = APPRX(
        double,
        second.bar,
        second.beat,
        second.separate);
    return FA < SA;
}


const BpmFragment& 
BpmStruct::getAffected(const BpmFragment& searchFrag)
const
{
    auto bpmIt = std::upper_bound(
        fragments.begin(), 
        fragments.end(),
        searchFrag,
        searchLambda
        );
    if(bpmIt == fragments.begin() || fragments.empty()){
        throw "empty bpm fragments. Runtime Err";
    }
    --bpmIt;
    return *bpmIt.base();
}

bool
FrameSearchLambda(const BpmFragment& first, const BpmFragment& second)
{
    return first.frame_to_here < second.frame_to_here;
}

const BpmFragment& 
BpmStruct::getAffected(const unsigned long long searchFrame)
const
{
    BpmFragment temp;
    temp.frame_to_here = searchFrame;
    auto bpmIt = std::upper_bound(
        fragments.begin(), 
        fragments.end(),
        temp,
        FrameSearchLambda
        );
    if(bpmIt == fragments.begin() || fragments.empty()){
        throw "empty bpm fragments. Runtime Err";
    }
    --bpmIt;
    return *bpmIt.base();
}

const std::vector<const BpmFragment*> 
BpmStruct::getAffectedList(
        const unsigned long long searchStartFrame,
        const unsigned long long searchEndFrame
    ) const
{
    BpmFragment Stemp;
    BpmFragment Etemp;

    Stemp.frame_to_here = searchStartFrame;
    Etemp.frame_to_here = searchEndFrame;
    auto StartIT = std::upper_bound(
        fragments.begin(), 
        fragments.end(),
        Stemp,
        FrameSearchLambda
        );
    if(StartIT == fragments.begin() || fragments.empty()){
        throw "empty bpm fragments. Runtime Err";
    }
    --StartIT;
    auto EndIT = std::upper_bound(
        fragments.begin(), 
        fragments.end(),
        Etemp,
        FrameSearchLambda
        );
    if(EndIT == fragments.begin() || fragments.empty()){
        throw "empty bpm fragments. Runtime Err";
    }
    --EndIT;
    if(StartIT == EndIT){
        return std::vector<const BpmFragment*>();
    }
    std::vector<const BpmFragment*> BRange;
    for(auto i = StartIT; i != std::next(EndIT); ++i){
        BRange.push_back(i.base());
    }
    return BRange;
}
