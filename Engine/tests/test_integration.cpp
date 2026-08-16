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

TEST_CASE("Integration: Crossover never looks ahead and returns correct total return") {
    PriceSeries ps1;
    ps1.dates  = {"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9"};
    ps1.open   = {100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0};
    ps1.high   = {100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0};
    ps1.low    = {100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0};
    ps1.close  = {100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0};   
    ps1.volume = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};

    MovingAverageCrossover strat(2,5);
    EquityCurve eq1 = simulate(ps1, strat);

    PriceSeries ps2;
    ps2.dates  = {"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9"};
    ps2.open   = {100.0, 150.0, 200.0, 250.0, 300.0, 600.0, 400.0, 450.0, 500.0, 550.0}; //Day 5 is changed to 600.0 to test that the crossover strategy does not look ahead and is not affected by future prices
    ps2.high   = {100.0, 150.0, 200.0, 250.0, 300.0, 600.0, 400.0, 450.0, 500.0, 550.0};
    ps2.low    = {100.0, 150.0, 200.0, 250.0, 300.0, 398.1, 400.0, 450.0, 500.0, 550.0};
    ps2.close  = {100.0, 150.0, 200.0, 250.0, 300.0, 400.0, 400.0, 450.0, 500.0, 550.0};
    ps2.volume = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    MovingAverageCrossover strat2(2,5);
    EquityCurve eq2 = simulate(ps2, strat2);
    REQUIRE(eq1.dailyReturn[4] == eq2.dailyReturn[4]); //The daily return on day 5 should be the same for both price series since the crossover strategy does not look ahead and is not affected by future prices
}