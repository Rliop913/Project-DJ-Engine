#include "MUSIC_CTR.hpp"

#define ORIGIN_TO_TARGET(TARGET, ORIGIN) (TARGET / ORIGIN)

void
Ingredients::SORT()
{
    std::sort(pos.begin(), pos.end(), [](const PlayPosition& first, const PlayPosition& second){
        return first.Gidx < second.Gidx;
    });

}

void
AddGBpm(std::vector<PlayPosition>& vec, BpmFragment fragment)
{
    PlayPosition pos;
    pos.Gidx = fragment.frame_to_here;
    pos.TargetBPM = fragment.bpm;
    pos.status = MIXSTATE::BPMCHANGE;
    vec.push_back(pos);
}

void
Ingredients::FillGlobal(std::vector<PlayPosition>& Gbpm, const BpmStruct& Global)
{
    auto GRes = Global.getAffectedList(pos.front().Gidx, pos.back().Gidx);
    
    if(GRes.empty()){
        auto FRes = Global.getAffected(pos.front().Gidx);
        AddGBpm(Gbpm, FRes);
    }
    else{
        for(auto i : GRes){
            AddGBpm(Gbpm, *i);
        }
    }
    pos.front().TargetBPM = Gbpm.front().TargetBPM;
    Gbpm.erase(Gbpm.begin());
}

void
Ingredients::FillLocal(std::vector<PlayPosition>& Lbpm, const BpmStruct& Local)
{
    for(unsigned int i=0; i<pos.size(); ++i){
        auto now = pos[i];
        if(now.status == MIXSTATE::PLAY){
            
            auto nextLidx = now.Lidx + 
                (pos[i+1].Gidx - now.Gidx) * ORIGIN_TO_TARGET(now.TargetBPM, now.OriginBPM);
            
            auto betweenBpm = Local.getAffectedList(now.Lidx, nextLidx);
            if(betweenBpm.empty()){
                auto LocalAffect = Local.getAffected(now.Lidx);
                pos[i].OriginBPM = LocalAffect.bpm;
            }
            else{
                for(const auto& j : betweenBpm){
                    if(j->frame_to_here <= now.Lidx){
                        pos[i].OriginBPM = j->bpm;
                    }
                    else{
                        auto FromOriginRange = j->frame_to_here - now.Lidx;
                        PlayPosition tempos;
                        tempos.OriginBPM = j->bpm;
                        tempos.Gidx = now.Gidx + FromOriginRange;
                        // tempos.Lidx = j->frame_to_here;
                        tempos.status = MIXSTATE::BPMCHANGE;
                        Lbpm.push_back(tempos);
                    }
                }
            }
        }
    }
}
#include <iostream>
std::vector<PlayPosition>::iterator
Ingredients::GetSameGidx(GLOBAL_POS gidx)
{
    return
    std::find_if(pos.begin(), pos.end(), [gidx](const PlayPosition& P){
        return P.Gidx == gidx;
    });
}

void
Ingredients::Ready(const BpmStruct& Global, const BpmStruct& Local)
{
    SORT();
    std::vector<PlayPosition> Gbpm;
    FillGlobal(Gbpm, Global);
    std::vector<PlayPosition> Lbpm;
    FillLocal(Lbpm, Local);

    for(const auto& i : Gbpm){
        auto matched = GetSameGidx(i.Gidx);
        if(matched != pos.end()){
            matched->TargetBPM = i.TargetBPM;
        }
        else{
            pos.push_back(i);
        }
    }
    for(const auto& i : Lbpm){
        auto matched = GetSameGidx(i.Gidx);
        if(matched != pos.end()){
            matched->OriginBPM = i.OriginBPM;
        }
        else{
            pos.push_back(i);
        }
    }
    SORT();
    double Stacked_Origin_BPM = -1;
    double Stacked_TargetBPM = -1;

    for(auto& i : pos){
        if(i.OriginBPM < 0){
            i.OriginBPM = Stacked_Origin_BPM;
        }
        else{
            Stacked_Origin_BPM = i.OriginBPM;
        }

        if(i.TargetBPM < 0){
            i.TargetBPM = Stacked_TargetBPM;
        }
        else{
            Stacked_TargetBPM = i.TargetBPM;
        }
    }
    
    for(unsigned int i=1; i<pos.size(); ++i){
        if(pos[i].status == MIXSTATE::BPMCHANGE){
            auto Range = pos[i].Gidx - pos[i - 1].Gidx;
            pos[i].Lidx = pos[i - 1].Lidx + (Range * ORIGIN_TO_TARGET(pos[i - 1].TargetBPM, pos[i - 1].OriginBPM));
            pos[i].status = MIXSTATE::PLAY;
        }
    }
    
}
