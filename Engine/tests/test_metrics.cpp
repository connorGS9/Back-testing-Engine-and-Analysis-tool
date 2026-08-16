#include "catch_amalgamated.hpp"
#include "EquityCurve.h"
#include "Metrics.h"
#include <cmath>

TEST_CASE("totalReturn computes net return over the curve") {
    EquityCurve eq;
    eq.dates       = {"d0", "d1", "d2", "d3"};
    eq.equity      = {100.0, 120.0, 150.0};   // initial 100, final 150
    eq.dailyReturn = {0.0, 0.10, 0.10, -0.1735537};  // day-over-day, day0 is 0

    REQUIRE(totalReturn(eq) == Catch::Approx(0.5));
}

TEST_CASE("Volatility correctly computes standard deviation of daily returns") {
    EquityCurve eq;
    eq.dates       = {"d0", "d1", "d2", "d3", "d4"};
    eq.equity      = {100.0, 120.0, 150.0, 180.0, 200.0};   // initial 100, final 200
    eq.dailyReturn = {0.0, 0.10, -0.10, 0.10, -0.10};  // day-over-day, day0 is 0
    double expected = 0.1 * sqrt(252.0);
    REQUIRE(volatility(eq) == Catch::Approx(expected));
}

TEST_CASE("Volatility returns 0.0 on one element EquityCurve") { // const std::size_t n = ec.dailyReturn.size() - 1; if (n == 0) return 0.0;
    EquityCurve eq;
    eq.dates       = {"d0"}; //One element series should return 0.0
    eq.equity      = {100.0}; 
    eq.dailyReturn = {0.0};  

    REQUIRE(volatility(eq) == 0.0);
}

TEST_CASE("Volatility handles a completely empty curve") { // if (ec.dailyReturn.empty()) { return 0.0; } //Guard against empty curve, volatility is 0.0 if no data
    EquityCurve eq;   // all vectors empty, size 0
    REQUIRE(volatility(eq) == 0.0);
}