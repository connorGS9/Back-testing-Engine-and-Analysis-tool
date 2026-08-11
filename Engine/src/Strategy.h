#pragma once
#include "PriceSeries.h"
#include <cstddef>

//Abstract interace Strategy will represent any one of our strategies and return the correct pointer
class Strategy {
    public: 
    virtual ~Strategy() = default; //Virtual destructor necessary for deleting our ownership pointer "Opposite of a Contructor(x,y,z)"

    // called once per bar, returns the desired position (e.g. target shares, or a signal)
    virtual double onBar(const PriceSeries& series, std::size_t i) = 0; //Full price series and a given day 'i'
    //What it returns. A double target position — "I want to hold this many shares" (or a normalized signal like +1/0/-1 for fully-in/out)
};  // In a buy and hold strategy for example every day we return the equivalent to "All in" since we are just holding forever - not selling or buying more