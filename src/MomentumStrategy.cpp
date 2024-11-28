#include "MomentumStrategy.hpp"

MomentumStrategy::MomentumStrategy(int momentumPeriod) : momentumPeriod(momentumPeriod) {}

void MomentumStrategy::loadData(const std::vector<double>& inputPrices) {
    prices = inputPrices;
}

void MomentumStrategy::generateSignals() {
    size_t n = prices.size();
    signals.resize(n, 0);
    for (size_t i = momentumPeriod; i < n; ++i) {
        double momentum = prices[i] - prices[i - momentumPeriod];
        if (momentum > 0) {
            signals[i] = 1; // Buy
        } else if (momentum < 0) {
            signals[i] = -1; // Sell
        }
    }
}

double MomentumStrategy::backtest(double initialCapital) {
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

