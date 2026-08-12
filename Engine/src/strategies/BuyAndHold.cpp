#include "BuyAndHold.h"
#include "../PriceSeries.h"

double BuyAndHold::onBar(const PriceSeries& series, std::size_t i) { // BAH::onBar indiates this is the onBar for BAH ONLY
    return 1.0; // Always return 1.0, indicating we want to hold all shares (fully invested) regardless of the day 'i'
}