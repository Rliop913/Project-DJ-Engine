#include "MixMachine.hpp"

template<>
bool
MixMachine::TypeWorks<TypeEnum::LOAD, MUSIC_CTR>
(MixStruct& ms, MUSIC_CTR& data, litedb& db)
{
    if(!data.setLOAD(ms.RP, db, ms.frame_in)){
        return false;
    }
    return true;
}

template<>
bool
MixMachine::TypeWorks<TypeEnum::LOAD, BattleDj>
(MixStruct& ms, BattleDj& data)
{
    data.StartPos = ms.frame_in;
    return true;
}

// template<>
// bool
// MixMachine::TypeWorks<TypeEnum::LOAD, FilterFAUST>
// (MixStruct& ms, FilterFAUST& data)
// {
//     data.StartPos = ms.frame_in;
//     return true;
// }




template<>
bool
MixMachine::TypeWorks<TypeEnum::BATTLE_DJ, BattleDj>
(MixStruct& ms, BattleDj& data)
{
    switch (ms.RP.getDetails()){
    case DetailEnum::SPIN:
        if(!data.Spin(ms)){
            return false;
        }
        break;
    case DetailEnum::REV:
        if(!data.Rev(ms)){
            return false;
        }
        break;
    case DetailEnum::PITCH:
        if(!data.Pitch(ms)){
            return false;
        }
        break;
    case DetailEnum::SCRATCH:
        if(!data.Scratch(ms)){
            return false;
        }
        break;
    default:
        return false;
    }
    return true;
}

template<>
bool
MixMachine::TypeWorks<TypeEnum::CONTROL>
(MixStruct& ms, MUSIC_CTR& data)
{
    data.PausePos = ms.frame_in;
    return true;
}

template<>
bool
MixMachine::TypeWorks<TypeEnum::UNLOAD>
(MixStruct& ms, MUSIC_CTR& data)
{
    data.FullPos = ms.frame_in;
    return true;
}

