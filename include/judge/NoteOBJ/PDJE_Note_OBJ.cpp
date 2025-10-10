#include "PDJE_Note_OBJ.hpp"
#include <algorithm>
namespace PDJE_JUDGE {

template <>
void
OBJ::Fill<IN>(const NOTE &data)
{
    in.push_back(data);
}

template <>
void
OBJ::Fill<OUT>(const NOTE &data)
{
    out.push_back(data);
}

void
OBJ::Sort()
{
    auto compare = [](const NOTE &a, const NOTE &b) { return a.pos < b.pos; };

    std::sort(in.begin(), in.end(), compare);
    std::sort(out.begin(), out.end(), compare);
    iitr = in.begin();
    oitr = out.begin();
}

template <>
void
OBJ::Get<IN>(unsigned long long limit, std::vector<NOTE *> &found)
{
    found.clear();
    while (true) { // pull iterator
        if (iitr != in.end() && iitr->used) {
            ++iitr;
        } else {
            break;
        }
    }
    auto titr = iitr;
    while (true) {
        if ((titr != in.end()) && titr->pos <= limit && !titr->used) {
            found.push_back(std::addressof(*titr));
            ++titr;
        } else {
            break;
        }
    }
}

template <>
void
OBJ::Get<OUT>(unsigned long long limit, std::vector<NOTE *> &found)
{
    found.clear();
    while (true) { // pull iterator
        if (oitr != in.end() && oitr->used) {
            ++oitr;
        } else {
            break;
        }
    }
    auto titr = oitr;
    while (true) {
        if ((titr != in.end()) && titr->pos <= limit && !titr->used) {
            found.push_back(std::addressof(*titr));
            ++titr;
        } else {
            break;
        }
    }
}

}; // namespace PDJE_JUDGE
