#include "audioRender.hpp"

bool
audioRender::LoadTrack(litedb& db, trackdata& td)
{
    auto searchRes = db << td;
    if(!searchRes.has_value()){
        return false;
    }
    auto mb = CapReader<MixBinaryCapnpData>();
    if(!mb.open(searchRes.value()[0].mixBinary)){
        return false;
    }
    auto mt = MixTranslator();
    if(!(mt.Read(mb))){
        return false;
    }
    auto mm = MixMachine();

    if(!mm.IDsort(mt)){
        return false;
    }
    
    if(!mm.mix(db, mt.bpms.value())){
        return false;
    }
    rendered_frames = std::move(mm.rendered_out);
    
    if(!rendered_frames.has_value()){
        return false;
    }
    return true;
}