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
    mm.IDsort(mt);
    mm.mix(db, mt.bpms.value());
    rendered_frames = std::move(mm.rendered_out);
    
    if(!rendered_frames.has_value()){
        return false;
    }
    return true;
    //todo - implement note reads
}