#include "catch_amalgamated.hpp"
#include "PriceSeries.h"
#include "Simulator.h"
#include "strategies/BuyAndHold.h"
#include "EquityCurve.h"
#include "Metrics.h"
#include <cmath>

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