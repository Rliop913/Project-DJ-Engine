#include "Bpm.hpp"


BPM::BPM()
{
    usable_threads = std::thread::hardware_concurrency();
    if(usable_threads == 0){
        usable_threads = 1;
    }
}

BPM::~BPM()
{

}

void 
bpm_thread(
    MixStruct* M, 
    std::mutex* bpm_locker, 
    std::vector<BpmStruct>* bpms, 
    unsigned long range
    )
{
    MixStruct* mp = M;
    for(unsigned long i=0; i<range; ++i){
        if(mp->RP.getType() == TypeEnum::BPM_CONTROL){
            auto bpmStr = std::string(mp->RP.getFirst().cStr());
            BpmStruct tempbpm;
            tempbpm.bar = mp->RP.getBar();
            tempbpm.beat = mp->RP.getBeat();
            tempbpm.separate = mp->RP.getSeparate();
            try
            {
                tempbpm.bpm = std::stod(bpmStr);
            }
            catch(...)
            {
                tempbpm.bpm = -404;
            }
            {
                std::lock_guard<std::mutex> lock(*bpm_locker);
                bpms->push_back(tempbpm);
            }
        }
        ++mp;
    }
}

bool
BPM::getBpms(MIX& mixx)
{
    unsigned long jobs_per_thread = mixx.mixVec.size() / usable_threads;
    if(jobs_per_thread == 0){
        std::mutex bpm_locker;
        bpm_thread(
            &(mixx.mixVec[0]),
            &bpm_locker,
            &(bpmVec),
            mixx.mixVec.size()
            );
    }
    else{
        unsigned long remained_job = mixx.mixVec.size() - (jobs_per_thread * usable_threads);
        std::vector<std::thread> thread_pool;
        unsigned long idx = 0;
        std::mutex bpm_locker;
        for(unsigned int i=0; i<(usable_threads - 1); ++i){
            thread_pool.emplace_back(
                bpm_thread, 
                &(mixx.mixVec[idx]),
                &bpm_locker,
                &(bpmVec),
                jobs_per_thread);
            idx += jobs_per_thread;
        }
        thread_pool.emplace_back(
            bpm_thread, 
            &(mixx.mixVec[idx]),
            &bpm_locker,
            &(bpmVec),
            jobs_per_thread + remained_job);
        
        for(int i =0 ; i<thread_pool.size(); ++i){
            thread_pool[i].join();
        }

    }
    if(
        bpmVec.empty() ||
        bpmVec[0].bar != 0 ||
        bpmVec[0].beat != 0
    ){
        return false;
    }
    for(auto i : bpmVec){
        if(i.bpm <= 0){
            return false;
        }
    }
    bpmVec[0].frame_to_here = 0;
    if(bpmVec.size() == 1){
        return true;
    }
    auto Sp = &(bpmVec[0]);
    auto Ep = &(bpmVec[1]);
    for(unsigned long i=1; i<bpmVec.size(); ++i){
        Ep->frame_to_here =
        Sp->frame_to_here +
        FrameCalc::CountFrame(
            Sp->bar, Sp->beat, Sp->separate,
            Ep->bar, Ep->beat, Ep->separate,
            Ep->bpm
        );
        ++Sp;
        ++Ep;
    }
    return true;
}