#include "catch_amalgamated.hpp"
#include "PriceSeries.h"
#include "Simulator.h"
#include "strategies/BuyAndHold.h"
#include "EquityCurve.h"
#include "Metrics.h"
#include <cmath>
#include "strategies/MovingAverageCrossover.h"

TEST_CASE("Integration: buy-and-hold return equals the asset's price return") {
    PriceSeries ps;
    ps.dates  = {"d0", "d1", "d2"};
    ps.open   = {100.0, 150.0, 200.0};
    ps.high   = {100.0, 150.0, 200.0};
    ps.low    = {100.0, 150.0, 200.0};
    ps.close  = {100.0, 150.0, 200.0};   // asset doubles 100 -> 200
    ps.volume = {1000, 1000, 1000};

    BuyAndHold strat;
    EquityCurve eq = simulate(ps, strat);

    REQUIRE(totalReturn(eq) == Catch::Approx(1.0));  // doubling = 100% return
}

TEST_CASE("Integration: transaction cost is charged on traded notional") {
    PriceSeries ps;
    ps.dates = {"d0"};
    ps.open = {100.0};
    ps.high = {100.0};
    ps.low = {100.0};
    ps.close = {100.0};
    ps.volume = {1000};

    BuyAndHold strat;
    EquityCurve eq = simulate(ps, strat, 0.01);

    // 1,000 shares * $100 * 1% = $1,000 cost.
    REQUIRE(eq.equity.back() == Catch::Approx(99000.0));
}

TEST_CASE("Integration: Crossover never looks ahead and returns correct total return") {
    PriceSeries base;
    base.dates  = {"d0","d1","d2","d3","d4"};
    base.open   = {100,110,105,115,120};
    base.high   = {100,110,105,115,120};
    base.low    = {100,110,105,115,120};
    base.close  = {100,110,105,115,120};
    base.volume = {1000,1000,1000,1000,1000};

    MovingAverageCrossover strat(2, 3);        // small windows so day 3 actually computes

    double before = strat.onBar(base, 3);      // decision at day 3 reads close[1..3]

    PriceSeries mutated = base;                // value-semantic copy
    mutated.close[4] = 999.0;                  // change a FUTURE bar (index 4 > 3)

    double after = strat.onBar(mutated, 3);    // same day, future altered

    REQUIRE(before == after);
}