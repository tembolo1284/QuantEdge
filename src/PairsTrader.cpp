#include "PairsTrader.hpp"
#include "Statistics.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

PairsTrader::PairsTrader(double zThreshold) : zThreshold(zThreshold) {}

void PairsTrader::loadData(const std::vector<double>& asset1, const std::vector<double>& asset2) {
    if (asset1.size() != asset2.size()) {
        throw std::invalid_argument("Assets must have the same number of data points.");
    }
    asset1Prices = asset1;
    asset2Prices = asset2;
}

void PairsTrader::loadDataFromFiles(const std::string& file1, const std::string& file2) {
    auto readPrices = [](const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        std::vector<double> prices;
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string date, priceStr;
            std::getline(ss, date, ',');  // Ignore the date
            std::getline(ss, priceStr, ',');
            prices.push_back(std::stod(priceStr));
        }
        return prices;
    };

    asset1Prices = readPrices(file1);
    asset2Prices = readPrices(file2);

    if (asset1Prices.size() != asset2Prices.size()) {
        throw std::invalid_argument("Files must have the same number of rows.");
    }
}

void PairsTrader::generateSignals() {
    size_t n = asset1Prices.size();
    std::vector<double> spread(n);
    for (size_t i = 0; i < n; ++i) {
        spread[i] = asset1Prices[i] - asset2Prices[i];
    }

    double spreadMean = Statistics::mean(spread);
    double spreadStdDev = Statistics::stddev(spread);

    signals.resize(n, 0);
    for (size_t i = 0; i < n; ++i) {
        double z = Statistics::zScore(spread[i], spreadMean, spreadStdDev);
        if (z > zThreshold) {
            signals[i] = -1; // Short spread
        } else if (z < -zThreshold) {
            signals[i] = 1; // Long spread
        }
    }
}

double PairsTrader::backtest(double initialCapital) {
    double capital = initialCapital;
    double position = 0.0;

    for (size_t i = 1; i < signals.size(); ++i) {
        if (signals[i] != 0) {
            position = signals[i] * (asset1Prices[i] - asset2Prices[i]);
        } else if (position != 0) {
            capital += position;
            position = 0.0;
        }
    }
    return capital;
}

