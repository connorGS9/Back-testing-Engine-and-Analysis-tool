#pragma once

#include <string>
#include "AssetConfig.h"

AssetConfig makeAssetConfig(const std::string& symbol);

bool isCrypto(const std::string& symbol);