#include "Loader.h"
#include <fstream>
#include <sstream>
#include "PriceSeries.h"
#include <stdexcept>

PriceSeries loadCsv(const std::string& path) {
    //Read from file, skip header row, split fields by commas, fill each vector (long date, double: open, high, low, close, long volume) parsing from string
   PriceSeries series;
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not create the ifStream from the given path:" + path);
    }
    std::string line;
    std::string field;
    std::getline(file, line); // Drop headers 
    while (std::getline(file, line)) { // While theres still lines to take in
        std::istringstream ss(line); // Change to string stream and reuse getline method with a delimitter (',') to parse by column
        std::getline(ss, field, ',');
        series.dates.push_back(field);
        std::getline(ss, field, ',');
        series.open.push_back(std::stod(field));
        std::getline(ss, field, ',');
        series.high.push_back(std::stod(field));
        std::getline(ss, field, ',');
        series.low.push_back(std::stod(field));
        std::getline(ss, field, ',');
        series.close.push_back(std::stod(field));
        std::getline(ss, field, ',');
        series.volume.push_back(std::stol(field));       
    }

    return series;

}
