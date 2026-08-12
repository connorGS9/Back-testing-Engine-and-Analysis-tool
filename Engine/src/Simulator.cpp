#include <vector>
#include <string>
#include "Simulator.h"
#include "EquityCurve.h"

EquityCurve simulate(const PriceSeries& series, Strategy& strategy) {
    Portfolio p { 0.0, 100000.0 };
    EquityCurve eq;

    for (std::size_t i = 0; i < series.size(); i++) {
        // DECIDE — recalculated every iteration
        double target = strategy.onBar(series, i); // Get the specific strategy 

        // ACT — move the portfolio toward that target
        double equityNow = p.cash + p.shares * series.close[i];
        double targetShares = (target * equityNow) / series.close[i];
        p.shares = targetShares;
        p.cash = equityNow - targetShares * series.close[i];

        // MARK & RECORD
        double equityToday = p.cash + p.shares * series.close[i];
        double ret = (i == 0) ? 0.0
                   : (equityToday - eq.equity[i-1]) / eq.equity[i-1];
        eq.dates.push_back(series.dates[i]);
        eq.equity.push_back(equityToday);
        eq.dailyReturn.push_back(ret);
    }
    return eq;
}