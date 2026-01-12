#include "PDJE_PreProcess.hpp"

namespace PDJE_JUDGE {

bool
PreProcess::GetDatas()
{
    if (init->inputline->input_arena) {
        init->inputline->input_arena->Receive();
        Parse(parsed_res, init->raildb, init->inputline->input_arena->datas);
    }
    if (init->inputline->midi_datas) {
        auto got = init->inputline->midi_datas->Get();
        Parse(parsed_res, init->raildb, *got);
    }
    if ((parsed_res.logs.empty() || init->inputline->input_arena == nullptr) &&
        (parsed_res.midi_logs.empty() ||
         init->inputline->midi_datas == nullptr)) {
        return false;
    } else {
        return true;
    }
}

void
PreProcess::Cut(const uint64_t cut_range)
{
    missed_buffers.clear();
    init->note_objects->Cut<BUFFER_MAIN>(cut_range, missed_buffers);

    if (!missed_buffers.empty()) {
        Event_Datas.miss_queue.Write(missed_buffers);
    }
    missed_buffers.clear();
    init->note_objects->Cut<BUFFER_SUB>(cut_range, missed_buffers);
    if (!missed_buffers.empty()) {

        Event_Datas.miss_queue.Write(missed_buffers);
    }
}

}; // namespace PDJE_JUDGE