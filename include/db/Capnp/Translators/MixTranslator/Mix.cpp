#include "Mix.hpp"
#include "Bpm.hpp"
#include "PDJE_LOG_SETTER.hpp"
MIX::MIX()
{
    usable_threads = std::thread::hardware_concurrency();
    if(usable_threads == 0){
        usable_threads = 1;
    }
}

MIX::~MIX()
{

}

bool
MIX::openMix(const MixBinaryCapnpData::Reader& Rptr)
{
    try
    {
        auto mixDatas = Rptr.getDatas();
        auto mixSize = mixDatas.size();

        mixVec.resize(mixSize);
        auto MP = &(mixVec[0]);
        for(unsigned long i=0; i<mixSize; ++i){
            MP->RP = mixDatas[i];
            ++MP;
        }
        std::sort(mixVec.begin(), mixVec.end(), [](const MixStruct& first, MixStruct second){
            auto F = 
            static_cast<double>(first.RP.getBar()) + (
                static_cast<double>(first.RP.getBeat()) /
                static_cast<double>(first.RP.getSeparate())
            );
            auto S = 
            static_cast<double>(second.RP.getBar()) + (
                static_cast<double>(second.RP.getBeat()) /
                static_cast<double>(second.RP.getSeparate())
            );
            return F < S;
        });
        return true;
    }
    catch(std::exception& e)
    {
        critlog("failed to open capnpMixdata. from MIX openMix. ExceptionLog: ");
        critlog(e.what());
        return false;
    }
}

/**
 * @brief calculates mixing data's frame position.
 * 
 * @param bs bpm fragment
 * @param B bpm class
 * @return unsigned long the frame position
 */
unsigned long
FillFrame(const BpmFragment& bs, BPM* B)
{
    auto bpmIt = B->bpmVec.getAffected(bs);
    return 
    FrameCalc::CountFrame(
        bpmIt.bar,
        bpmIt.beat,
        bpmIt.separate,
        bs.bar,
        bs.beat,
        bs.separate,
        bpmIt.bpm
    ) + bpmIt.frame_to_here;
}

/**
 * @brief preprocessing for mix datas.
 * 
 * @param M Mixing datas.
 * @param B Bpm datas.
 * @param range render range to mix
 */
void
mix_thread(
    MixStruct* M,
    BPM* B,
    unsigned long range
)
{
    for(unsigned long i=0; i<range; ++i){
        BpmFragment bsin;
        BpmFragment bsout;
        bsin.bar = M->RP.getBar();
        bsin.beat = M->RP.getBeat();
        bsin.separate = M->RP.getSeparate();
        bsout.bar = M->RP.getEbar();
        bsout.beat = M->RP.getEbeat();
        bsout.separate = M->RP.getEseparate();
        M->frame_in = FillFrame(bsin, B);
        M->frame_out = FillFrame(bsout, B);
        ++M;
    }
}

bool
MIX::WriteFrames(BPM& bpmm)
{
    unsigned long jobs_per_thread = mixVec.size() / usable_threads;
    if(jobs_per_thread == 0){
        mix_thread(
            &(mixVec[0]),
            &(bpmm),
            mixVec.size()
        );
    }
    else{
        unsigned long remained_job = mixVec.size() - (jobs_per_thread * usable_threads);
        std::vector<std::thread> thread_pool;
        unsigned long idx = 0;
        for(unsigned int i=0; i<(usable_threads - 1); ++i){
            thread_pool.emplace_back(
                mix_thread, 
                &(mixVec[idx]),
                &(bpmm),
                jobs_per_thread);
            idx += jobs_per_thread;
        }
        thread_pool.emplace_back(
            mix_thread, 
            &(mixVec[idx]),
            &(bpmm),
            jobs_per_thread + remained_job);
        
        for(int i =0 ; i<thread_pool.size(); ++i){
            thread_pool[i].join();
        }
    }


    return true;
}