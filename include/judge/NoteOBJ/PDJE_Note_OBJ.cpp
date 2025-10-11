#include "PDJE_Note_OBJ.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
namespace PDJE_JUDGE {

template <>
void
OBJ::Fill<IN>(const NOTE &data, uint64_t rail_id)
{
    in[rail_id].vec.push_back(data);
}

template <>
void
OBJ::Fill<OUT>(const NOTE &data, uint64_t rail_id)
{
    out[rail_id].vec.push_back(data);
}

void
OBJ::Sort()
{
    auto compare = [](const NOTE &a, const NOTE &b) { return a.pos < b.pos; };

    for (auto &i : in) {

        std::sort(i.second.vec.begin(), i.second.vec.end(), compare);
        i.second.itr = i.second.vec.begin();
    }
    for (auto &o : out) {
        std::sort(o.second.vec.begin(), o.second.vec.end(), compare);
        o.second.itr = o.second.vec.begin();
    }
}

template <>
void
OBJ::Get<IN>(const uint64_t limit, uint64_t railID, P_NOTE_VEC &found)
{
    found.clear();
    auto rail = in[railID];

    while (true) { // pull iterator
        if (rail.itr != rail.vec.end() && rail.itr->used) {
            ++rail.itr;
        } else {
            break;
        }
    }

    auto titr = in[railID].itr;
    while (true) {
        if ((titr != rail.vec.end()) && titr->pos <= limit && !titr->used) {
            found.push_back(std::addressof(*titr));
            ++titr;
        } else {
            break;
        }
    }
}

template <>
void
OBJ::Get<OUT>(const uint64_t limit, uint64_t railID, P_NOTE_VEC &found)
{
    found.clear();
    auto rail = out[railID];

    while (true) { // pull iterator
        if (rail.itr != rail.vec.end() && rail.itr->used) {
            ++rail.itr;
        } else {
            break;
        }
    }

    auto titr = out[railID].itr;
    while (true) {
        if ((titr != rail.vec.end()) && titr->pos <= limit && !titr->used) {
            found.push_back(std::addressof(*titr));
            ++titr;
        } else {
            break;
        }
    }
}
template <>
void
OBJ::Cut<IN>(const unsigned long long                limit,
             std::unordered_map<uint64_t, NOTE_VEC> &cuts)
{
    cuts.clear();
    for (auto &rail : in) {
        auto titr = rail.second.itr;
        while (titr != rail.second.vec.end() && titr->pos <= limit) {
            if (!titr->used) {
                cuts[rail.first].push_back(*titr);
                titr->used = true;
            }
            ++titr;
        }
    }
}

template <>
void
OBJ::Cut<OUT>(const unsigned long long                limit,
              std::unordered_map<uint64_t, NOTE_VEC> &cuts)
{
    cuts.clear();
    for (auto &rail : out) {
        auto titr = rail.second.itr;
        while (titr != rail.second.vec.end() && titr->pos <= limit) {
            if (!titr->used) {
                cuts[rail.first].push_back(*titr);
                titr->used = true;
            }
            ++titr;
        }
    }
}

}; // namespace PDJE_JUDGE
