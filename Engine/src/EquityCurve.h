#pragma once
#include <vector>
#include <string>

struct EquityCurve {
    std::vector<std::string> dates;
    std::vector<double> equity;
    std::vector<double> dailyReturn;
};