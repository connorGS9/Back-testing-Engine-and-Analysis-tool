#pragma once
#include "PriceSeries.h"
#include "EquityCurve.h"
#include "strategies/Strategy.h"

struct Portfolio {
    double shares;
    double cash;
};

// costRate is the fraction of traded notional charged as transaction cost.
// A zero default preserves the original no-cost simulation behavior.
EquityCurve simulate(const PriceSeries& series, Strategy& strategy,
                     double costRate = 0.0);