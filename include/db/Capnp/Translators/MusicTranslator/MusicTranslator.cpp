#include "MusicTranslator.hpp"
#include "PDJE_LOG_SETTER.hpp"
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
            catch(std::exception& e)
            {
                critlog("failed to convert string to double. from MusicTranslator Read. ErrString: ");
                critlog(DVec[i].getBpm().cStr());
                continue;
            }
            bpms.fragments.push_back(frg);
        }
    }
    bpms.sortFragment();
    
    return bpms.calcFrame(startFrame);
}