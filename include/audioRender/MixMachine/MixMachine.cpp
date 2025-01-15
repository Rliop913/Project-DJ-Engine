#include "MixMachine.hpp"


MixMachine::MixMachine(const unsigned int ch)
{
    num_channel = ch;
}

bool
MixMachine::operator<<(const MixBinary<READ_MODE::READ_ONLY>& binary)
{
    auto DPTR = reinterpret_cast<MixBinaryCapnpData::Reader*>(binary.D)->getDatas();
    try
    {    
        for(unsigned long i=0; i<DPTR.size(); ++i){
            long id = DPTR[i].getId();
            if(Memorized.find(id) == Memorized.end()){
                Memorized[id] = std::vector<MBData::Reader>();
            }
            Memorized[id].push_back(DPTR[i]);
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}


bool
MixMachine::mix(litedb& db)
{
    auto num_threads = Memorized.size();
    //todo - implement mix

}