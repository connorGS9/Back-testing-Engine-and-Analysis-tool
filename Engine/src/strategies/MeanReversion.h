#pragma once
#include "Strategy.h"

class MeanReversion : public Strategy {
public:
    MeanReversion(std::size_t window, double threshold);
    double onBar(const PriceSeries& series, std::size_t i) override;
    std::string name() const override;

private:
    std::size_t window_;
    double threshold_;
};