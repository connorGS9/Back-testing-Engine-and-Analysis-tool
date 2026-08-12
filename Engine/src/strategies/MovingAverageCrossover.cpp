#include "MovingAverageCrossover.h"
#include "../PriceSeries.h"
#include <iostream>


MovingAverageCrossover::MovingAverageCrossover(std::size_t shortWindow, std::size_t longWindow)
    : shortWindow_(shortWindow), longWindow_(longWindow) {}

double MovingAverageCrossover::onBar(const PriceSeries& ps, std::size_t index) {
    if (index + 1 < longWindow_) {
        return 0.0; // Not enough data to calculate long moving average
    }

    double averageShort = 0.0;
    for (std::size_t start = index + 1 - shortWindow_; start <= index; start++) {
        averageShort += ps.close[start];
    }
    averageShort /= shortWindow_; //Average of all closes in the past shortWindow days
    
    double averageLong = 0.0;
    for (std::size_t longStart = index + 1 - longWindow_; longStart <= index; longStart++) {
        averageLong += ps.close[longStart];
    }
    averageLong /= longWindow_; //Average of all closes in the past longWindow days
    
    return averageShort > averageLong ? 1.0 : 0.0; //Return 1 if (invested) or 0 for cash (not invested) based on the crossover of the short and long moving averages
}