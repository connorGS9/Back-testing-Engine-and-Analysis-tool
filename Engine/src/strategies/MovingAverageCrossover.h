#pragma once
#include "Strategy.h"

class MovingAverageCrossover : public Strategy {
public:
    MovingAverageCrossover(std::size_t shortWindow, std::size_t longWindow);
    double onBar(const PriceSeries& ps, std::size_t index) override;
    std::string name() const override {
        return "crossover_" + std::to_string(shortWindow_) + "_" + std::to_string(longWindow_);
    }
private:
    std::size_t shortWindow_;
    std::size_t longWindow_;
};