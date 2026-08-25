#pragma once
#include "Simulator.h"
#include "AssetConfig.h"

struct Metrics {
    double totalReturn;
    double annualizedReturn;
    double volatility;
    double sharpe;
    double maxDrawdown;
    double sortino;
};

Metrics computeMetrics(const EquityCurve& ec, const AssetConfig& assetConfig);
double totalReturn(const EquityCurve& ec);
double annualizedReturn(const EquityCurve& ec, const AssetConfig& assetConfig);
double volatility(const EquityCurve& ec, const AssetConfig& assetConfig);
double sharpe(const EquityCurve& ec, const AssetConfig& assetConfig);
double maxDrawdown(const EquityCurve& ec);
double sortino(const EquityCurve& ec, const AssetConfig& assetConfig);