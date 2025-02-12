#include "MixMachine.hpp"

template<>
bool
MixMachine::TypeWorks<TypeEnum::FILTER, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    data.filterData.emplace_back(Vec, ms.frame_in, ms.frame_out);

    TRY(
        data.filterData.back().selectInterpolator =
        std::stoi(ms.RP.getFirst().cStr());
    )
    if(data.filterData.back().selectInterpolator == InterpolateType::FLAT){
        TRY(
            data.filterData.back().vZero =
            std::stof(ms.RP.getSecond().cStr());
        )
    }
    else{
        EightPointValues EPV(ms.RP.getSecond().cStr());
        data.filterData.back().v1 = EPV.vals[0];
        data.filterData.back().v2 = EPV.vals[1];
        data.filterData.back().v3 = EPV.vals[2];
        data.filterData.back().v4 = EPV.vals[3];
        data.filterData.back().v5 = EPV.vals[4];
        data.filterData.back().v6 = EPV.vals[5];
        data.filterData.back().v7 = EPV.vals[6];
        data.filterData.back().v8 = EPV.vals[7];
    }
    switch (ms.RP.getDetails())
    {
    case DetailEnum::HIGH:
        
        //치명적인 논리 오류. 이 부분 무조건 레이스 컨디션 발생함. 로직을 고쳐야함.
        //방법 1. 순차적으로 작업을 요하는 부분만 예외처리 : 복잡함.-> 잡 큐에 넣을때 순차 판별 코드 제작.
        //방법 2. 전부 순차적으로 돌리기 : 너무 저성능임
        //방법 1 추진.
        // data.filterData.back().LowFilterSW()
        /* code */
        break;
    
    default:
        break;
    }
}