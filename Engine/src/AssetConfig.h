#pragma once
#include <string>

struct AssetConfig {
    std::string symbol = "";
    double tradingDaysPerYear = 252.0;   // 252 equities, 365 crypto
    double transactionCostRate = 0.0;  // per-trade proportional cost
    std::string currency = "USD";        // for later cross-asset work
};