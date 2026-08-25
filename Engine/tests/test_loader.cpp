#include <fstream>
#include <cstdio>
#include "PriceSeries.h"
#include "catch_amalgamated.hpp"
#include <Loader.h>

TEST_CASE("Loader parses columns into the right fields") {
    // write a tiny known CSV to a temp file
    const std::string path = "test_tmp_AAPL.csv";
    std::ofstream f(path);
    f << "Date,Open,High,Low,Close,Volume\n";
    f << "2020-01-02,10.0,12.0,9.0,11.0,1000\n";
    f << "2020-01-03,11.0,13.0,10.5,12.5,2000\n";
    f.close();

    PriceSeries ps = loadCsv(path);
    std::remove(path.c_str());   // clean up

    REQUIRE(ps.close.size() == 2);
    REQUIRE(ps.open[0]  == Catch::Approx(10.0));
    REQUIRE(ps.high[0]  == Catch::Approx(12.0));
    REQUIRE(ps.low[0]  == Catch::Approx(9.0));
    REQUIRE(ps.close[0] == Catch::Approx(11.0));   // <-- catches column-order bugs
    REQUIRE(ps.close[1] == Catch::Approx(12.5));
}

TEST_CASE("Loader throws on a missing file") {
    REQUIRE_THROWS(loadCsv("this_file_does_not_exist_12345.csv"));
}