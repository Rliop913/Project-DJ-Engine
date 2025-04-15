#pragma once


#include "jsonWrapper.hpp"
#include "gitWrapper.hpp"
#include "dbRoot.hpp"

class PDJE_Editor{
private:
    PDJE_JSONHandler JHandle;
public:
    PDJE_Editor() = default;
    ~PDJE_Editor() = default;
};