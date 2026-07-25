#pragma once
#include <vector>
#include <string> 
#include <cstddef>

struct PriceSeries {
    // struct of vectors for each data column. singular index for all data of a single day of a given price series
    std::vector<std::string> dates;
    std::vector<double> open;
    std::vector<double> high;
    std::vector<double> low;
    std::vector<double> close;
    std::vector<long> volume;

    std::size_t size() const { return close.size(); } // quick method to get size of close (# of recorded trading days) for validating
};