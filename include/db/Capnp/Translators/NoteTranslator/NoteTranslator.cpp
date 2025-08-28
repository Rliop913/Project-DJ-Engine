#include "NoteTranslator.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <string>

bool
NoteTranslator::Read(const CapReader<NoteBinaryCapnpData> &binary,
                     const BpmStruct                      &mainBpm,
                     OBJ_SETTER_CALLBACK                  &lambdaCallback)
{
    if (!lambdaCallback) {
        warnlog(
            "return false because lambda is empty. from NoteTranslator Read");
        return false;
    }
    auto      br = binary.Rp->getDatas();
    BpmStruct bs;

    bs.fragments = noteBpms.fragments;
    for (size_t i = 0; i < br.size(); ++i) {
        if (strcmp(br[i].getNoteType().cStr(), "BPM") == 0) {
            auto fg     = BpmFragment();
            fg.beat     = br[i].getBeat();
            fg.subBeat  = br[i].getSubBeat();
            fg.separate = br[i].getSeparate();
            try {
                fg.bpm = std::stod(br[i].getFirst().cStr());
            } catch (std::exception &e) {
                critlog("failed to convert string to double. from "
                        "NoteTranslator Read. ExceptionLog: ");
                critlog(br[i].getFirst().cStr());
                continue;
            }
            bs.fragments.push_back(fg);
        }
    }
    bs.sortFragment();
    if (!bs.calcFrame()) {
        critlog("failed to calculate frames. from NoteTranslator Read.");
        return false;
    }
    for (size_t i = 0; i < br.size(); ++i) {
        if (strcmp(br[i].getNoteType().cStr(), "BPM") != 0) {
            BpmFragment searchfragment;
            searchfragment.beat        = br[i].getBeat();
            searchfragment.subBeat     = br[i].getSubBeat();
            searchfragment.separate    = br[i].getSeparate();
            auto               affects = bs.getAffected(searchfragment);
            unsigned long long position =
                affects.frame_to_here +
                FrameCalc::CountFrame(affects.beat,
                                      affects.subBeat,
                                      affects.separate,
                                      searchfragment.beat,
                                      searchfragment.subBeat,
                                      searchfragment.separate,
                                      affects.bpm);

            unsigned long long pos2;
            if (br[i].getESeparate() < 0) {
                pos2 = 0;
            } else {
                BpmFragment secondpos;
                secondpos.beat     = br[i].getEbeat();
                secondpos.subBeat  = br[i].getEsubBeat();
                secondpos.separate = br[i].getESeparate();
                auto res           = bs.getAffected(secondpos);
                pos2               = res.frame_to_here +
                       FrameCalc::CountFrame(res.beat,
                                             res.subBeat,
                                             res.separate,
                                             searchfragment.beat,
                                             searchfragment.subBeat,
                                             searchfragment.separate,
                                             res.bpm);
            }
            lambdaCallback(std::string(br[i].getNoteType().cStr()),
                           std::string(br[i].getNoteDetail().cStr()),
                           std::string(br[i].getFirst().cStr()),
                           std::string(br[i].getSecond().cStr()),
                           std::string(br[i].getThird().cStr()),
                           position,
                           pos2);
        }
    }
    return true;
}