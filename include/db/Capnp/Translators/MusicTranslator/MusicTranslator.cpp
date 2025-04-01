#include "MusicTranslator.hpp"
#include <string>
bool
MusicTranslator::Read(const CapReader<MusicBinaryCapnpData>& binary, unsigned long long startFrame)
{
    auto DVec = binary.Rp->getDatas();
    for(unsigned long i=0; i<DVec.size(); ++i){
        if(DVec[i].hasBpm()){
            BpmFragment frg;
            frg.bar = DVec[i].getBar();
            frg.beat = DVec[i].getBeat();
            frg.separate = DVec[i].getSeparate();
            try
            {
                frg.bpm = std::stod(DVec[i].getBpm().cStr());
                
            }
            catch(...)
            {
                continue;
            }
            bpms.fragments.push_back(frg);
        }
    }
    bpms.sortFragment();
    
    return bpms.calcFrame(startFrame);
}