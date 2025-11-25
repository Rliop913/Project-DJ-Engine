#include "InputParser.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Rule.hpp"
#include <algorithm>
#include <cstdint>

namespace PDJE_JUDGE {
PARSE_OUT *
InputParser::Parse(const INPUT_RAW &raw)
{
    outCache.logs.clear();
    outCache.logs.reserve(raw.second);
    if (raw.second == 0 || raw.first == nullptr) {
        return nullptr;
    }

    PDJE_Input_Log *p;
    int64_t         off;
    std::string     offsetkey;
    for (uint64_t i = 0; i < raw.second; ++i) {
        p = &raw.first[i];
        offsetkey.assign(p->id, p->id_len);
        auto it = offsetData.find(offsetkey);
        if (it != offsetData.end()) {
            off = it->second.offset_microsecond;
        } else {
            off = 0;
        }

        outCache.logs.push_back(raw.first[i]);
        outCache.logs.back().microSecond += off;
    }
    std::sort(outCache.logs.begin(),
              outCache.logs.end(),
              [](const PDJE_Input_Log &a, const PDJE_Input_Log &b) {
                  return a.microSecond < b.microSecond;
              });
    outCache.highest = outCache.logs.back().microSecond;
    outCache.lowest  = outCache.logs.front().microSecond;
    return &outCache;
}
} // namespace PDJE_JUDGE