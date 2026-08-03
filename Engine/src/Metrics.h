#pragma once
#include "Simulator.h"

struct Metrics {
    double totalReturn;
    double annualizedReturn;
    double volatility;
    double sharpe;
    double maxDrawdown;
    double sortino;
};

Metrics computeMetrics(const EquityCurve& ec);
double totalReturn(const EquityCurve& ec);
double annualizedReturn(const EquityCurve& ec);
double volatility(const EquityCurve& ec);
double sharpe(const EquityCurve& ec);
double maxDrawdown(const EquityCurve& ec);
double sortino(const EquityCurve& ec);