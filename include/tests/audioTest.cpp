
#include "MixBinary.hpp"
#include "MixTranslator.hpp"
#include <iostream>

#include <algorithm>

void fillDatas(capnp::List<MBData, capnp::Kind::STRUCT>::Builder& ret)
{
    ret[0].setBar(0);
    ret[0].setBeat(0);
    ret[0].setSeparate(4);
    ret[0].setEbar(0);
    ret[0].setEbeat(0);
    ret[0].setEseparate(4);
    ret[0].setType(TypeEnum::BPM_CONTROL);
    ret[0].setDetails(DetailEnum::SOLA_ON);
    ret[0].setFirst("60.0");
    ret[0].setId(0);

    ret[1].setBar(1);
    ret[1].setBeat(0);
    ret[1].setSeparate(4);
    ret[1].setEbar(1);
    ret[1].setEbeat(1);
    ret[1].setEseparate(4);

    ret[2].setBar(2);
    ret[2].setBeat(2);
    ret[2].setSeparate(4);
    ret[2].setEbar(2);
    ret[2].setEbeat(2);
    ret[2].setEseparate(4);
    
    ret[3].setBar(3);
    ret[3].setBeat(3);
    ret[3].setSeparate(4);
    ret[3].setEbar(3);
    ret[3].setEbeat(3);
    ret[3].setEseparate(4);
    ret[3].setType(TypeEnum::BPM_CONTROL);
    ret[3].setDetails(DetailEnum::SOLA_ON);
    ret[3].setFirst("140.0");
    ret[3].setId(0);
    
    ret[4].setBar(4);
    ret[4].setBeat(4);
    ret[4].setSeparate(4);
    ret[4].setEbar(4);
    ret[4].setEbeat(4);
    ret[4].setEseparate(4);
    
    ret[5].setBar(5);
    ret[5].setBeat(0);
    ret[5].setSeparate(4);
    ret[5].setEbar(5);
    ret[5].setEbeat(0);
    ret[5].setEseparate(4);
    
    ret[6].setBar(6);
    ret[6].setBeat(0);
    ret[6].setSeparate(4);
    ret[6].setEbar(6);
    ret[6].setEbeat(0);
    ret[6].setEseparate(4);
    
    ret[6].setType(TypeEnum::BPM_CONTROL);
    ret[6].setDetails(DetailEnum::SOLA_ON);
    ret[6].setFirst("120.0");
    ret[6].setId(0);

    ret[7].setBar(7);
    ret[7].setBeat(0);
    ret[7].setSeparate(4);
    ret[7].setEbar(7);
    ret[7].setEbeat(0);
    ret[7].setEseparate(4);
    
    ret[8].setBar(8);
    ret[8].setBeat(0);
    ret[8].setSeparate(4);
    ret[8].setEbar(8);
    ret[8].setEbeat(0);
    ret[8].setEseparate(4);
    
    ret[9].setBar(9);
    ret[9].setBeat(0);
    ret[9].setSeparate(4);
    ret[9].setEbar(9);
    ret[9].setEbeat(0);
    ret[9].setEseparate(4);
    ret[9].setType(TypeEnum::BPM_CONTROL);
    ret[9].setDetails(DetailEnum::SOLA_ON);
    ret[9].setFirst("90.5");
    ret[9].setId(0);
}


int
main()
{
    MixBinary mb = MixBinary<READ_WRITE>();
    mb.open();
    auto B = reinterpret_cast<MixBinaryCapnpData::Builder*>(mb.D);
    auto ret = B->initDatas(10);
    fillDatas(ret);
    auto flat_returned = mb.out();

    MixBinary rb = MixBinary<READ_MODE::READ_ONLY>();
    rb.open(flat_returned);
    auto reader = reinterpret_cast<MixBinaryCapnpData::Reader*>(rb.D);
    auto readGet = reader->getDatas();
    std::cout<<readGet[0].getId() << std::endl;
    std::cout<<readGet[1].getId() << std::endl;
    std::cout<<ret[0].getFirst().cStr() << std::endl;

    struct vectest{
        unsigned int one;
        unsigned int two;
    };
    std::vector<vectest> arrs(11);
    for(int i =0; i < 11; ++i){
        arrs[i].one = 20 - i * 2;
    }
    std::sort(arrs.begin(), arrs.end(), [](vectest first, vectest second){
        return first.one < second.one;
    });
    for( auto i : arrs){
        std::cout << i.one << std::endl;
    }
    for(int i=0; i<30; ++i){

        vectest tet;
        tet.one = i;
        auto it =std::upper_bound(arrs.begin(), arrs.end(),tet, [](vectest first, vectest second){
            return first.one < second.one;
        });
        --it;
        std::cout << i << " ID: " << it->one << std::endl;
    }
    
    MixTranslator mt = MixTranslator();
    if(!(mt<<rb)){
        return 1;
    }
    for(auto i : mt.mixs->mixVec){
        std::cout << "Frame In: " << i.frame_in << "Frame Out: " << i.frame_out << std::endl;
    }
    auto trp = reinterpret_cast<MixBinaryCapnpData::Reader*>(rb.D);
    auto dds = trp->getDatas();
    auto dPTR = (dds[1]);
    std::cout<<"origin: " << dds[1].getBar() << "copy: " << dPTR.getBar() << std::endl;
    
    std::cout<<"origin: " << dds[1].getBar() << "copy: " << dPTR.getBar() << std::endl;
    
    return 0;
}