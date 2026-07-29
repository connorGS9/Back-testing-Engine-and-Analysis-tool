#include <iostream>
#include "Loader.h"
#include "Simulator.h"
#include "Writer.h"

int main() {
      PriceSeries series = loadCsv("/mnt/c/Users/conno/OneDrive/Desktop/Back-testing-Engine-and-Analysis-tool/data/AAPL.csv");

      std::cout<< "Rows loaded: " << series.size() << "\n";

      std::cout << "First: " << series.dates[0] << " O:" << series.open[0]
          << " H:" << series.high[0] << " L:" << series.low[0]
          << " C:" << series.close[0] << " V:" << series.volume[0] << "\n";

      std::size_t last = series.size() - 1;
      std::cout << "Last:  " << series.dates[last] << " O:" << series.open[last]
          << " C:" << series.close[last] << " V:" << series.volume[last] << "\n";


      EquityCurve eq = simulate(series);

      std::cout << "Day 0 Equity: " << eq.equity[0] <<"\n";
      std::cout << "Final day of equity: " << eq.equity[eq.equity.size()-1] << "\n";

      writeCsv(eq, "/mnt/c/Users/conno/OneDrive/Desktop/Back-testing-Engine-and-Analysis-tool/data/SIMULATION_OPT.csv");

    return 0;
}