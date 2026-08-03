#pragma once
#include "PriceSeries.h"
#include "EquityCurve.h"

struct Portfolio {
    double shares;
    double cash;
};


EquityCurve simulate(const PriceSeries& series);