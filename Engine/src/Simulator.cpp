#include <vector>
#include <string>
#include "Simulator.h"

EquityCurve simulate(const PriceSeries& series) {
    Portfolio p { 0.0, 100000.0 };
    EquityCurve eq;

    for (std::size_t i = 0; i < series.size(); i++) {
        if (i == 0) {
            p.shares = p.cash / series.close[0];
            p.cash = 0;
        }

        eq.dates.push_back(series.dates[i]);
        eq.equity.push_back(p.cash + p.shares * series.close[i]);
        double ret = (i == 0) ? 0.0
           : (eq.equity[i] - eq.equity[i-1]) / eq.equity[i-1];
        eq.dailyReturn.push_back(ret);
    }
    return eq;
}