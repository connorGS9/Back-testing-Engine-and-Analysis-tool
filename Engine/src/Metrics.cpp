#include "EquityCurve.h"
#include "Metrics.h"
#include <stdexcept>
#include "Metrics.h"
#include <cmath>
#include <algorithm>


Metrics computeMetrics(const EquityCurve& ec) {
    Metrics m;
    m.totalReturn = totalReturn(ec);
    m.annualizedReturn = annualizedReturn(ec);
    m.volatility = volatility(ec);
    m.sharpe = sharpe(ec);
    m.maxDrawdown = maxDrawdown(ec);
    m.sortino = sortino(ec);
    return m;
}
double totalReturn(const EquityCurve& ec) {
    if (ec.equity.empty()) {
        throw std::runtime_error("Cannot compute return on empty curve");
    }
    const double initial = ec.equity[0];
    const double end = ec.equity[ec.equity.size() - 1];
    const double total = (end - initial) / initial;
    return total;
}

double annualizedReturn(const EquityCurve& ec) {
    // M = (1 - r)^N  Where r is the annual return and N is the length (years), and M is the growth factor (10.xx for Apple)
    // r = m^(1/n) - 1
    //annual return is really just a geometric average for "average yearly return" if you go +100% one year and -50% the next annual return will be 0% even though changes did occur to the total money at certain times
    const double N = ec.equity.size() / 252.0; //Trading days -> years (252 trading days per year avg)
    const double M = ec.equity[ec.equity.size()-1] / ec.equity[0];
    const double annualReturn = std::pow(M, 1.0 / N) - 1;
    return annualReturn;
}

double volatility(const EquityCurve& ec) {
    // Volatility is how far a typical day lands from the average day on average both positively and negatively
    // V = sqrt(variance)  variance = average of (dailyReturn - mean)^2 over all i
    double mean = 0.0;
    double sumDailyReturns = 0.0;
    const std::size_t n = ec.dailyReturn.size() - 1;
    if (n == 0) return 0.0;
    for (std::size_t i = 1; i < n + 1; i++) {
        sumDailyReturns += ec.dailyReturn[i];
    }
    mean = sumDailyReturns / n;
    double variance = 0.0;
    for (std::size_t i = 1; i < n + 1; i++) {
        double d = ec.dailyReturn[i] - mean;
        variance += d * d;
    }
    variance /= n;
    return std::sqrt(variance) * std::sqrt(252.0);
}

double sharpe(const EquityCurve& ec) {
    // Sharpe returns annualReturn / volatility "How much return per unit of risk taken"
    constexpr double RISK_FREE = 0.04; // 4% return on something very safe like short term t-bills, used to make the sharpe evaluation more realistic if we returned 26% annualy, we could have gotten 4% "risk free" so we really mesure the return to be 22%
    const double AR = annualizedReturn(ec);
    const double ANNVOL = volatility(ec); // Daily volatility * sqrt(# tradingDays) = annual volatility
    return (AR - RISK_FREE) / ANNVOL;
}

double maxDrawdown(const EquityCurve& ec) {
    // Max drawdown is the worst peak-to-trough drop the equity ever suffered before a new peak was hit
    double peak = ec.equity[0];
    double maxDraw = 0.0;
    for (std::size_t i = 1; i < ec.equity.size(); i++) {
        if (ec.equity[i] > peak) peak = ec.equity[i]; // Running max
        maxDraw = std::max(maxDraw, (peak - ec.equity[i]) / peak); // Max draw is biggest drop from peak to low in percentages
    }
    return maxDraw;
}

double sortino(const EquityCurve& ec) {
    double mean = 0.0;
    double negativeReturnSum = 0.0;
    const std::size_t n = ec.dailyReturn.size() - 1;
    if (n == 0) return 0.0;

    for (std::size_t i = 1; i < n + 1; i++) {
        if (ec.dailyReturn[i] < 0.0) {
            negativeReturnSum += ec.dailyReturn[i] *  ec.dailyReturn[i];
        }
    }

    const double downsideDev = std::sqrt(negativeReturnSum / n);
    const double annualizedDownside = downsideDev * std::sqrt(252.0);

    const double riskFree = 0.04;
    const double annReturn = annualizedReturn(ec);

    if (annualizedDownside == 0.0) return 0.0;   // guard
    return (annReturn - riskFree) / annualizedDownside;
}