#pragma once
#include "InputParser.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_Judge_Loop_Structs.hpp"
#include "PDJE_RAIL.hpp"
#include "PDJE_SYNC_CORE.hpp"
namespace PDJE_JUDGE {

class PreProcess {
  private:
    Judge_Init *init;

    bool
                                           GetDatas();
    std::unordered_map<uint64_t, NOTE_VEC> missed_buffers;

  public:
    Queues        Event_Datas;
    audioSyncData synced_data;
    uint64_t      local_micro_pos;
    uint64_t      global_local_diff;
    uint64_t      use_range;
    PARSE_OUT     parsed_res;
    PreProcess(Judge_Init *initp)
    {
        init = initp;
    }
    void
    Cut(const uint64_t cut_range);
    bool
    Work();

    ~PreProcess() = default;
};
}; // namespace PDJE_JUDGE