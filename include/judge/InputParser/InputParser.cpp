// #include "InputParser.hpp"
// #include "PDJE_Input_DataLine.hpp"
// #include "PDJE_Rule.hpp"
// #include <algorithm>
// #include <cstdint>

// namespace PDJE_JUDGE {
// PARSE_OUT *
// InputParser::Parse(const INPUT_RAW &raw)
// {
//     outCache.logs.clear();
//     outCache.logs.reserve(raw.size());
//     if (raw.size() == 0) {
//         return nullptr;
//     }

//     PDJE_Input_Log *p;
//     int64_t         off;
//     std::string     offsetkey;
//     for (const auto &rawp : raw) {
//         offsetkey.assign(rawp.id, rawp.id_len);
//         auto it = offsetData.find(offsetkey);
//         if (it != offsetData.end()) {
//             off = it->second.offset_microsecond;
//         } else {
//             off = 0;
//         }
//         outCache.logs.push_back(rawp);
//         outCache.logs.back().microSecond += off;
//     }
//     std::sort(outCache.logs.begin(),
//               outCache.logs.end(),
//               [](const PDJE_Input_Log &a, const PDJE_Input_Log &b) {
//                   return a.microSecond < b.microSecond;
//               });
//     outCache.highest = outCache.logs.back().microSecond;
//     outCache.lowest  = outCache.logs.front().microSecond;
//     return &outCache;
// }
// } // namespace PDJE_JUDGE