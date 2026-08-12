#include <iostream>
#include "Loader.h"
#include "Simulator.h"
#include "EquityCurve.h"
#include "Writer.h"
#include "Metrics.h"
#include <cmath>
#include <memory>                      
#include "strategies/BuyAndHold.h"
#include "strategies/MovingAverageCrossover.h"
#include <iomanip>

int main(int argc, char** argv) {

    if (argc < 3) { // Improper number of arguments 
        std::cerr << "Usage: engine <TICKER> <strategy> [params...]\n";
        std::cerr << "  e.g. engine AAPL buyhold\n";
        std::cerr << "       engine TSLA crossover 50 200\n";
        return 1;   // error exit
    }

    std::string ticker = argv[1];
    std::string strategyName = argv[2];

    PriceSeries series = loadCsv("../data/" + ticker + ".csv");
    // Hardcoded if else for now when there are > 4 strategies we will upgrade to factory method pattern to create the correct strategy object based on the string name of the strategy
    std::unique_ptr<Strategy> strategy;
    if (strategyName == "buyhold") {
        strategy = std::make_unique<BuyAndHold>();
    } else if (strategyName == "crossover") {
        if (argc < 5) {
            std::cerr << "Usage: engine <TICKER> crossover <short> <long>\n";
            return 1;
        }
        int shortMa = std::stoi(argv[3]);
        int longMa = std::stoi(argv[4]);
        std::cout << "Running crossover " << shortMa << "/" << longMa << '\n';
        strategy = std::make_unique<MovingAverageCrossover>(shortMa, longMa);
    } else {
        std::cerr << "Unknown strategy: " << strategyName << "\n";
        return 1;
    }

    EquityCurve eq = simulate(series, *strategy);      
    Metrics m = computeMetrics(eq);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== Backtest Results ===\n";
    std::cout << "Days simulated:    " << eq.equity.size() << "\n";
    std::cout << "Starting equity:   " << eq.equity[0] << "\n";
    std::cout << "Final equity:      " << eq.equity[eq.equity.size() - 1] << "\n";
    std::cout << "\n--- Metrics ---\n";
    std::cout << "Total return:      " << m.totalReturn * 100.0      << "%\n";
    std::cout << "Annualized return: " << m.annualizedReturn * 100.0 << "%\n";
    std::cout << "Volatility (ann):  " << m.volatility * 100.0       << "%\n";
    std::cout << "Sharpe:            " << m.sharpe                    << "\n";
    std::cout << "Sortino:           " << m.sortino                  << "\n";
    std::cout << "Max drawdown:      " << m.maxDrawdown * 100.0       << "%\n";

    // Write the equity curve to a CSV file for analysis in Jupiter Notebook and Python
    //writeCsv(eq, "output/" + ticker + "_" + strategyName + ".csv");
    return 0;
}