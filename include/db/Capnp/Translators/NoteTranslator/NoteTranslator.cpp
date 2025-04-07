#include "NoteTranslator.hpp"
#include <string>

bool
NoteTranslator::Read(
    const CapReader<NoteBinaryCapnpData>& binary, 
    const BpmStruct& mainBpm,
    OBJ_SETTER_CALLBACK& lambdaCallback
    )
{
    if(!lambdaCallback){
        return false;
    }
    auto br = binary.Rp->getDatas();
    BpmStruct bs;

    bs.fragments = noteBpms.fragments;    
    for(size_t i=0; i < br.size(); ++i){
        if(strcmp(br[i].getNoteType().cStr(), "BPM") == 0){
            auto fg= BpmFragment();
            fg.bar = br[i].getBar();
            fg.beat = br[i].getBeat();
            fg.separate = br[i].getSeparate();
            try
            {
                fg.bpm =
                std::stod(br[i].getFirst().cStr());
            }
            catch(...)
            {
                continue;
            }
            bs.fragments.push_back(fg);
        }
    }
    bs.sortFragment();
    if(!bs.calcFrame()){
        return false;
    }
    for(size_t i=0; i < br.size(); ++i){
        if(strcmp(br[i].getNoteType().cStr(), "BPM") != 0){
            BpmFragment searchfragment;
            searchfragment.bar = br[i].getBar();
            searchfragment.beat = br[i].getBeat();
            searchfragment.separate = br[i].getSeparate();
            auto affects = bs.getAffected(searchfragment);
            unsigned long long position =
            affects.frame_to_here +
            FrameCalc::CountFrame(
                affects.bar,
                affects.beat,
                affects.separate,
                searchfragment.bar,
                searchfragment.beat,
                searchfragment.separate,
                affects.bpm
            );
            
            unsigned long long pos2;
            if(br[i].getESeparate() < 0){
                pos2 = 0;
            }
            else{
                BpmFragment secondpos;
                secondpos.bar = br[i].getEBar();
                secondpos.beat = br[i].getEBeat();
                secondpos.separate = br[i].getESeparate();
                auto res = bs.getAffected(secondpos);
                pos2 =
                res.frame_to_here +
                FrameCalc::CountFrame(
                    res.bar,
                    res.beat,
                    res.separate,
                    searchfragment.bar,
                    searchfragment.beat,
                    searchfragment.separate,
                    res.bpm
                );
            }
            lambdaCallback(
                std::string(br[i].getNoteType().cStr()),
                std::string(br[i].getNoteDetail().cStr()),
                std::string(br[i].getFirst().cStr()),
                std::string(br[i].getSecond().cStr()),
                std::string(br[i].getThird().cStr()),
                position,
                pos2
            );
        }
    }
    return true;
}