#include "MomentumStrategy.hpp"
#include <iostream>

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

    // Debug: Print generated signals
    std::cout << "Momentum Signals: ";
    for (int signal : signals) {
        std::cout << signal << " ";
    }
    std::cout << std::endl;
}

double MomentumStrategy::backtest(double initialCapital) {
    double capital = initialCapital;
    double position = 0.0;

    for (size_t i = 1; i < signals.size(); ++i) {
        if (signals[i] == 1) { // Buy
            position += prices[i];
            std::cout << "Buy at " << prices[i] << ", position: " << position << "\n";
        } else if (signals[i] == -1) { // Sell
            capital += position - prices[i];
            position = 0.0;
            std::cout << "Sell at " << prices[i] << ", capital: " << capital << "\n";
        }
    }

    // Add remaining position value to capital
    if (position > 0) {
        capital += position;
    }
    return capital;
}
