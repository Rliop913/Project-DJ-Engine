#include "FrameCalc.hpp"

#include "PDJE_LOG_SETTER.hpp"

namespace FrameCalc {
uint64_t
CountFrame(uint64_t Sbeat,
           uint64_t SsubBeat,
           uint64_t Sseparate,
           uint64_t Ebeat,
           uint64_t EsubBeat,
           uint64_t Eseparate,
           double   bpm)
{
    Sseparate   = Sseparate > 0 ? Sseparate : 1;
    Eseparate   = Eseparate > 0 ? Eseparate : 1;
    bpm         = bpm > 0 ? bpm : 1;
    auto Sapprx = APPRX(double, Sbeat, SsubBeat, Sseparate);
    auto Eapprx = APPRX(double, Ebeat, EsubBeat, Eseparate);
    if (Sapprx > Eapprx) {
        critlog("Failed to Count Frame. Start apprx position is bigger than "
                "End apprx position. Start apprx: ");
        critlog(Sapprx);
        critlog("End apprx: ");
        critlog(Eapprx);
    }
    return static_cast<unsigned long>(
        std::round((Eapprx - Sapprx) * (DMINUTE / bpm) * DSAMPLERATE));
}
} // namespace FrameCalc

bool
sortLambda(const BpmFragment &first, const BpmFragment &second)
{
    auto F =
        static_cast<double>(first.beat) + (static_cast<double>(first.subBeat) /
                                           static_cast<double>(first.separate));
    auto S = static_cast<double>(second.beat) +
             (static_cast<double>(second.subBeat) /
              static_cast<double>(second.separate));
    return F < S;
}

void
BpmStruct::sortFragment()
{
    if (fragments.size() > 1) {
        std::sort(fragments.begin(), fragments.end(), sortLambda);
    }
}

bool
BpmStruct::calcFrame(unsigned long long StartPos)
{
    if (fragments.size() > 0) {
        for (auto i : fragments) {
            if (i.bpm <= 0) {
                critlog(
                    "bpm safe check failed. from BpmStruct calcFrame. bpm: ");
                std::string bpmLog = std::to_string(i.bpm);
                critlog(bpmLog);
                return false;
            }
        }
        fragments[0].frame_to_here = StartPos;
        if (fragments.size() > 1) {
            auto Sp = &(fragments[0]);
            auto Ep = &(fragments[1]);
            for (unsigned long i = 1; i < fragments.size(); ++i) {
                Ep->frame_to_here =
                    Sp->frame_to_here + FrameCalc::CountFrame(Sp->beat,
                                                              Sp->subBeat,
                                                              Sp->separate,
                                                              Ep->beat,
                                                              Ep->subBeat,
                                                              Ep->separate,
                                                              Sp->bpm);
                ++Sp;
                ++Ep;
            }
        }
        fragments[0].frame_to_here = 0;
        return true;
    } else {
        critlog("bpm fragment data is empty. from BpmStruct calcFrame.");
        return false;
    }
}

bool
searchLambda(const BpmFragment &first, const BpmFragment &second)
{
    double FA = APPRX(double,
                      first.beat,
                      first.subBeat,
                      (first.separate == 0 ? 1 : first.separate));
    double SA = APPRX(double,
                      second.beat,
                      second.subBeat,
                      (second.separate == 0 ? 1 : second.separate));
    return FA < SA;
}

const BpmFragment &
BpmStruct::getAffected(const BpmFragment &searchFrag) const
{
    auto bpmIt = std::upper_bound(
        fragments.begin(), fragments.end(), searchFrag, searchLambda);
    if (bpmIt == fragments.begin() || fragments.empty()) {
        critlog("cannot get affected bpm. empty bpm fragments. from BpmStruct "
                "getAffected-bpmfragment");
    }
    --bpmIt;
#ifdef __WINDOWS__
    return *bpmIt;
#endif
#ifndef __WINDOWS__
    return *bpmIt.base();
#endif
    // return *bpmIt.base();
}

bool
FrameSearchLambda(const BpmFragment &first, const BpmFragment &second)
{
    return first.frame_to_here < second.frame_to_here;
}

const BpmFragment &
BpmStruct::getAffected(const unsigned long long searchFrame) const
{
    BpmFragment temp;
    temp.frame_to_here = searchFrame;
    auto bpmIt         = std::upper_bound(
        fragments.begin(), fragments.end(), temp, FrameSearchLambda);
    if (bpmIt == fragments.begin() || fragments.empty()) {
        critlog("cannot get affected bpm. empty bpm fragments. from BpmStruct "
                "getAffected-ull");
    }
    --bpmIt;
#ifdef __WINDOWS__
    return *bpmIt;
#endif
// todo - check these codes and watch diffs
#ifndef __WINDOWS__
    return *bpmIt.base();
#endif
}

const std::vector<const BpmFragment *>
BpmStruct::getAffectedList(const unsigned long long searchStartFrame,
                           const unsigned long long searchEndFrame) const
{
    BpmFragment Stemp;
    BpmFragment Etemp;

    Stemp.frame_to_here = searchStartFrame;
    Etemp.frame_to_here = searchEndFrame;
    auto StartIT        = std::upper_bound(
        fragments.begin(), fragments.end(), Stemp, FrameSearchLambda);
    if (StartIT == fragments.begin() || fragments.empty()) {
        critlog("cannot get affected bpm. empty bpm fragments. from BpmStruct "
                "getAffectedList-StartIT");
    }
    --StartIT;
    auto EndIT = std::upper_bound(
        fragments.begin(), fragments.end(), Etemp, FrameSearchLambda);
    if (EndIT == fragments.begin() || fragments.empty()) {
        critlog("cannot get affected bpm. empty bpm fragments. from BpmStruct "
                "getAffectedList-EndIT");
    }
    --EndIT;
    if (StartIT == EndIT) {
        infolog("StartIT and EndIT is same. from BpmStruct getAffectedList");
        return std::vector<const BpmFragment *>();
    }
    std::vector<const BpmFragment *> BRange;
    for (auto i = StartIT; i != std::next(EndIT); ++i) {
#ifdef __WINDOWS__
        BRange.push_back(&(*i));
#endif
// Also here
#ifndef __WINDOWS__

        BRange.push_back(i.base());
#endif
    }
    return BRange;
}
