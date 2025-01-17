#include "MixTranslator.hpp"

MixTranslator::MixTranslator()
{
    usable_threads = std::thread::hardware_concurrency();
    if(usable_threads == 0){
        usable_threads = 1;
    }
    mixs.emplace();
    bpms.emplace();
}


bool
MixTranslator::Read(const MixBinary<READ_MODE::READ_ONLY>& binary)
{
    if(!mixs->openMix(reinterpret_cast<MixBinaryCapnpData::Reader*>(binary.D))){
        return false;
    }
    if(!bpms->getBpms(mixs.value())){
        return false;
    }
    if(!mixs->WriteFrames(bpms.value())){
        return false;
    }
    return true;
    // if(!WriteFrames()){
    //     return false;
    // }
}

MixTranslator::~MixTranslator()
{
    
}