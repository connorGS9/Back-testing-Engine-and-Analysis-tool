#pragma once
#include "Strategy.h"

class BuyAndHold : public Strategy {
public:
    double onBar(const PriceSeries& series, std::size_t i) override;
    std::string name() const override {
        return "BuyAndHold";
    }
};