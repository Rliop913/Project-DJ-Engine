#pragma once

#include <optional>
#include <vector>

class BattleDj{
private:

public:
    BattleDj();
    ~BattleDj();
    std::optional<std::vector<float>>
    Execute(std::optional<std::vector<float>>& Array);
};