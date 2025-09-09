#pragma once

#include "dbRoot.hpp"

template<typename DataType>
class PDJE_Linter{
private:

public:
    static bool Lint(const DataType& target, UNSANITIZED& lint_msg);
};