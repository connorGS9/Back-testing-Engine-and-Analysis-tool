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
#include "strategies/MeanReversion.h"
#include <iomanip>
#include "AssetConfig.h"
#include "AssetRegistry.h"
#include <unordered_map>
#include <functional>
#include <string>

int main(int argc, char** argv) {

    if (argc < 3) { // Improper number of arguments 
        std::cerr << "Usage: engine <TICKER> <strategy> [params...]\n";
        std::cerr << "  e.g. engine AAPL buyhold\n";
        std::cerr << "       engine TSLA crossover 50 200\n";
        return 1;   // error exit
    }

    std::string ticker = argv[1];
    std::string strategyName = argv[2];

    AssetConfig assetConfig = makeAssetConfig(ticker); // Create an asset config for the given ticker

    PriceSeries series = loadCsv("../data/" + ticker + ".csv");
    // Hardcoded if else for now when there are > 4 strategies we will upgrade to factory method pattern to create the correct strategy object based on the string name of the strategy
    std::unordered_map<std::string, std::function<std::unique_ptr<Strategy>(int, char**)>> registry;
    registry["buyhold"] = [](int, char**) -> std::unique_ptr<Strategy> { return std::make_unique<BuyAndHold>(); };
    registry["crossover"] = [](int argc, char** argv) -> std::unique_ptr<Strategy> {
        if (argc < 5) {
            std::cerr << "Usage: engine <TICKER> crossover <short> <long>\n";
            return std::unique_ptr<Strategy>(nullptr);
        }
        int shortMa = std::stoi(argv[3]);
        int longMa = std::stoi(argv[4]);
        return std::make_unique<MovingAverageCrossover>(shortMa, longMa);
    };
    registry["meanrev"] = [](int argc, char** argv) -> std::unique_ptr<Strategy> { 
        if (argc < 5) {
            std::cerr << "Usage: engine <TICKER> meanrev <window> <threshold of std_deviations away formm mean price (e.g. 1, 2)>\n";
        }
        int window = std::stoi(argv[3]);
        int thresh = std::stoi(argv[4]);
        return std::make_unique<MeanReversion>(window, thresh);
    };

    auto it = registry.find(strategyName); // Map iterator to find the strategy name in the registry
    std::unique_ptr<Strategy> strategy;
    if (it == registry.end()) { // strategy name does not exist in map
        std::cerr << "Unknown strategy: " << strategyName << "\n";
        return 1; // error exit
    } else {
        strategy = it->second(argc, argv); // the strategy is created and stored in a unique pointer to ensure proper memory management and avoid memory leaks. The unique pointer will automatically delete the strategy object when it goes out of scope, ensuring that resources are properly released.
    }
    if (!strategy) { // strategy creation failed due to invalid parameters
        return 1; // error exit
    }

    // TODO: select this rate from the ticker's stock type/exchange metadata.
    // Example: use a higher rate for illiquid penny/OTC stocks.
    double costRate = 0.001; // 0.1% cost rate assumption for now
    EquityCurve eq = simulate(series, *strategy, costRate);
    Metrics m = computeMetrics(eq, assetConfig);

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
    std::string outPath = "../output/" + ticker + "_" + strategy->name() + ".csv";
    writeCsv(eq, outPath);
    return 0;
}