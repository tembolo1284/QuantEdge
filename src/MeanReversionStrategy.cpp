#include "MeanReversionStrategy.hpp"
#include "Statistics.hpp"

MeanReversionStrategy::MeanReversionStrategy(int shortWindow, int longWindow)
    : shortWindow(shortWindow), longWindow(longWindow) {}

void MeanReversionStrategy::loadData(const std::vector<double>& inputPrices) {
    prices = inputPrices;
}

void MeanReversionStrategy::generateSignals() {
    size_t n = prices.size();
    signals.resize(n, 0);
    for (size_t i = longWindow; i < n; ++i) {
        double shortMean = Statistics::mean(std::vector<double>(prices.begin() + i - shortWindow, prices.begin() + i));
        double longMean = Statistics::mean(std::vector<double>(prices.begin() + i - longWindow, prices.begin() + i));

        if (prices[i] < shortMean) {
            signals[i] = 1; // Long
        } else if (prices[i] > longMean) {
            signals[i] = -1; // Short
        }
    }
}

double MeanReversionStrategy::backtest(double initialCapital) {
    double capital = initialCapital;
    double position = 0.0;

    for (size_t i = 1; i < signals.size(); ++i) {
        if (signals[i] != 0) {
            position = signals[i] * prices[i];
        } else if (position != 0) {
            capital += position;
            position = 0.0;
        }
    }
    return capital;
}

