#include "AssetConfig.h"
#include "AssetRegistry.h"
#include <string>

AssetConfig makeAssetConfig(const std::string& symbol) {
     if (isCrypto(symbol)) return AssetConfig{symbol, 365.0, 0.001, "USD"}; // Default values for crypto
    // if (isFtx(symbol)) return AssetConfig{symbol, 260.0, 0.001, inferCurrency(symbol)}; // Default values for FTX

    return AssetConfig{symbol, 252.0, 0.001, "USD"}; // Default values for US equity
}

bool isCrypto(const std::string& symbol) {
    return symbol == "ETH-USD" || symbol == "BTC-USD";
}