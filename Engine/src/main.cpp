#include <iostream>
#include "Loader.h"
#include "Simulator.h"
#include "EquityCurve.h"
#include "Writer.h"
#include "Metrics.h"
#include <cmath>

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
      
      std::size_t initial = eq.equity[0];
      std::size_t end = eq.equity[eq.equity.size() - 1];
      double calcEquity = totalReturn(eq);
      std::cout << "Day 0 equity : " << initial << " Ending equity: " << end << "\n"<< " Equity percent gain from method: " << calcEquity * 100.0 << "%" << "\n";
      double vol = volatility(eq);
      std::cout << "Daily volatility: " << vol << "\n" << "Annual volatility: " << vol * sqrt(252.0) << "\n";
   
      std::cout << "Sharpe evaluation: " << sharpe(eq) << "\n";

      double maxDraw = maxDrawdown(eq);
      std::cout << "Maximum draw: " << maxDraw * 100 << "%" << "\n";
      return 0;
}