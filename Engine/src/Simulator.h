#pragma once
#include "PriceSeries.h"
#include <vector>
#include <string>

struct Portfolio {
    double shares;
    double cash;
};

struct EquityCurve {
    std::vector<std::string> dates;
    std::vector<double> equity;
    std::vector<double> dailyReturn;
};

EquityCurve simulate(const PriceSeries& series);