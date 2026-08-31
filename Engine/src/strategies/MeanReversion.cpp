#include "MeanReversion.h"
#include <cmath>
#include <sstream>
#include <iomanip>

MeanReversion::MeanReversion(std::size_t window, double threshold)
    : window_(window), threshold_(threshold) {}

std::string MeanReversion::name() const {
    std::ostringstream oss;
    oss << "meanrev_" << window_ << "_"
        << std::fixed << std::setprecision(2) << threshold_;
    return oss.str();
}

double MeanReversion::onBar(const PriceSeries& series, std::size_t i) {
    if (i + 1 < window_) return 0.0; // Not enough data yet

    double mean = 0.0;
    for (std::size_t index = i + 1 - window_; index <= i; index++) {
        mean += series.close[index]; // Accumulate last window's days of prices
    }
    mean /= window_;

    double std_deviation = 0.0;
    for (std::size_t index = i + 1 - window_; index <= i; index++) {
        std_deviation += std::pow((series.close[index] - mean), 2);
    }
    std_deviation = std::sqrt(std_deviation / window_);
    if (std_deviation == 0.0) return 0.0; // Identical prices across whole window 
    double z_score = (series.close[i] - mean) / std_deviation;

    if (z_score < -threshold_) {
        return 1.0;
    }
    return 0.0;
}