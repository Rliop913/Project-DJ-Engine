
#include "MixBinary.hpp"
#include <iostream>
int
main()
{
    MixBinary mb = MixBinary<READ_WRITE>();
    mb.open();
    auto B = reinterpret_cast<MixBinaryCapnpData::Builder*>(mb.D);
    auto ret = B->initDatas(2);
    ret[0].setId(10);
    ret[1].setId(11);
    ret[0].setFirst("test");
    auto flat_returned = mb.out();

    MixBinary rb = MixBinary<READ_MODE::READ_ONLY>();
    rb.open(flat_returned);
    auto reader = reinterpret_cast<MixBinaryCapnpData::Reader*>(rb.D);
    auto readGet = reader->getDatas();
    std::cout<<readGet[0].getId() << std::endl;
    std::cout<<readGet[1].getId() << std::endl;
    std::cout<<ret[0].getFirst().cStr() << std::endl;
    
    
    return 0;
}