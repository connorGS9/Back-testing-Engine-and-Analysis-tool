#include "catch_amalgamated.hpp"
#include "EquityCurve.h"
#include "Metrics.h"
#include <cmath>
#include "AssetConfig.h"

TEST_CASE("totalReturn computes net return over the curve") {
    EquityCurve eq;
    eq.dates       = {"d0", "d1", "d2", "d3"};
    eq.equity      = {100.0, 120.0, 150.0, 200.0};   // initial 100, final 200
    eq.dailyReturn = {0.0, 0.10, 0.10, -0.1735537};  // day-over-day, day0 is 0

    REQUIRE(totalReturn(eq) == Catch::Approx(1.0));
}

TEST_CASE("Volatility correctly computes standard deviation of daily returns") {
    EquityCurve eq;
    eq.dates       = {"d0", "d1", "d2", "d3", "d4"};
    eq.equity      = {100.0, 120.0, 150.0, 180.0, 200.0};   // initial 100, final 200
    eq.dailyReturn = {0.0, 0.10, -0.10, 0.10, -0.10};  // day-over-day, day0 is 0
    AssetConfig assetConfig{"TEST", 252.0, 0.001, "USD"}; // 252 trading days per year
    double expected = 0.1 * std::sqrt(252.0);
    REQUIRE(volatility(eq, assetConfig) == Catch::Approx(expected));
}

TEST_CASE("Volatility returns 0.0 on one element EquityCurve") { // const std::size_t n = ec.dailyReturn.size() - 1; if (n == 0) return 0.0;
    EquityCurve eq;
    eq.dates       = {"d0"}; //One element series should return 0.0
    eq.equity      = {100.0}; 
    eq.dailyReturn = {0.0};  
    AssetConfig assetConfig{"TEST", 252.0, 0.001, "USD"}; // 252 trading days per year
    REQUIRE(volatility(eq, assetConfig) == 0.0);
}

TEST_CASE("Volatility handles a completely empty curve") { // if (ec.dailyReturn.empty()) { return 0.0; } //Guard against empty curve, volatility is 0.0 if no data
    EquityCurve eq;   // all vectors empty, size 0
    AssetConfig assetConfig{"TEST", 252.0, 0.001, "USD"}; // 252 trading days per year
    REQUIRE(volatility(eq, assetConfig) == 0.0); // Default values for US equity
}

TEST_CASE("maxDrawdown finds the worst peak-to-trough drop") {
    EquityCurve eq;
    eq.dates       = {"d0","d1","d2","d3"};
    eq.equity      = {100.0, 150.0, 75.0, 120.0};   // peak 150, trough 75
    eq.dailyReturn = {0.0, 0.50, -0.50, 0.60};
    // worst drop: 150 -> 75 = (150-75)/150 = 0.50
    REQUIRE(maxDrawdown(eq) == Catch::Approx(0.50));
}

TEST_CASE("annualizedReturn compounds the total return over the period") {
    EquityCurve eq;
    // 252 trading days would be exactly 1 year; use a curve that doubles.
    // With a 2-day curve, years = 2/252, so annualized = 2^(252/2) - 1 (astronomically large),
    // which is a bad test. Instead use enough days that the math is sane and hand-checkable.
    eq.dates.assign(253, "d");
    eq.equity.assign(253, 0.0);
    eq.dailyReturn.assign(253, 0.0);
    for (int i = 0; i < 253; i++) eq.equity[i] = 100.0;   // flat
    eq.equity[252] = 200.0;                                // ends at double on the last day
    AssetConfig cfg{"TEST", 252.0, 0.0, "USD"};
    // 253 rows -> ~1.004 years; multiple = 2.0; annualized ~= 2^(1/1.004) - 1 ~= 0.994
    REQUIRE(annualizedReturn(eq, cfg) == Catch::Approx(std::pow(2.0, 252.0/253.0) - 1.0));
}

TEST_CASE("sharpe divides excess return by volatility") {
    EquityCurve eq;
    eq.dates       = {"d0","d1","d2","d3","d4"};
    eq.equity      = {100.0, 110.0, 99.0, 108.9, 98.01};
    eq.dailyReturn = {0.0, 0.10, -0.10, 0.10, -0.10};
    AssetConfig cfg{"TEST", 252.0, 0.0, "USD"};
    // This asserts sharpe is internally consistent with its own pieces:
    // (annualizedReturn - riskFree) / volatility, using YOUR risk-free constant.
    // Because return/vol here are messy to hand-compute, we check consistency instead.
    double annRet = annualizedReturn(eq, cfg);
    double vol    = volatility(eq, cfg);
    double riskFree = 0.04;   // <-- match YOUR hardcoded risk-free rate
    REQUIRE(sharpe(eq, cfg) == Catch::Approx((annRet - riskFree) / vol));
}

TEST_CASE("sortino uses only downside deviation, so it exceeds sharpe here") {
    EquityCurve eq;
    eq.dates       = {"d0","d1","d2","d3","d4"};
    eq.equity      = {100.0, 130.0, 128.7, 167.31, 165.6};
    eq.dailyReturn = {0.0, 0.30, -0.01, 0.30, -0.01};   // big ups, tiny downs
    AssetConfig cfg{"TEST", 252.0, 0.0, "USD"};
    // With large upside and small downside, downside deviation < total volatility,
    // so sortino > sharpe. This asserts the RELATIONSHIP, which is the invariant.
    REQUIRE(sortino(eq, cfg) > sharpe(eq, cfg));
}

