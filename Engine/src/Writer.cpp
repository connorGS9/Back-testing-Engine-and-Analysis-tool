#include "Writer.h"
#include <fstream>
#include <stdexcept>
#include <string>
#include <iomanip>

void writeCsv(const EquityCurve& eq, const std::string& path) {
    std::ofstream file(path);
    if (!file) {
        throw std::runtime_error("Could not find path to write to");
    }
    file << std::fixed << std::setprecision(6); 
    file << "Date,Equity,DailyReturn\n";
    for (std::size_t i = 0; i < eq.equity.size(); i++) {
        file << eq.dates[i] << "," << eq.equity[i] << "," << eq.dailyReturn[i]<< "\n";
    }
}