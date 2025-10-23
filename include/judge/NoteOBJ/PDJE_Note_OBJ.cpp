#include "PDJE_Note_OBJ.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
namespace PDJE_JUDGE {

void
OBJ::Sort()
{
    auto compare = [](const NOTE &a, const NOTE &b) {
        return a.microsecond < b.microsecond;
    };

    for (auto &o : Buffer_Main) {
        std::sort(o.second.vec.begin(), o.second.vec.end(), compare);
        o.second.itr = o.second.vec.begin();
    }
    for (auto &o : Buffer_Sub) {
        std::sort(o.second.vec.begin(), o.second.vec.end(), compare);
        o.second.itr = o.second.vec.begin();
    }
}

}; // namespace PDJE_JUDGE
