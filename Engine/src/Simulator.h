#pragma once
#include "PriceSeries.h"
#include "EquityCurve.h"
#include "strategies/Strategy.h"

struct Portfolio {
    double shares;
    double cash;
};

EquityCurve simulate(const PriceSeries& series, Strategy& strategy);