#pragma once
#include <string>
#include <vector>
namespace PDJE_JUDGE {
constexpr bool IN  = true;
constexpr bool OUT = false;
struct NOTE {
    std::string        type;
    std::string        detail;
    std::string        first;
    std::string        second;
    std::string        third;
    unsigned long long pos  = 0;
    bool               used = false;
};

class OBJ {
  private:
    std::vector<NOTE>           in;
    std::vector<NOTE>::iterator iitr;
    std::vector<NOTE>           out;
    std::vector<NOTE>::iterator oitr;

  public:
    void
    Sort();

    template <bool>
    void
    Fill(const NOTE &data);

    template <bool>
    void
    Get(const unsigned long long limit, std::vector<NOTE *> &found);

    template <bool>
    void
    Cut(const unsigned long long limit, std::vector<NOTE> &cuts);
    OBJ()  = default;
    ~OBJ() = default;
};
}; // namespace PDJE_JUDGE