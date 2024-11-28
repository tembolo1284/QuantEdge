#include <iostream>
#include "PairsTrader.hpp"
#include "MeanReversionStrategy.hpp"
#include "MomentumStrategy.hpp"

int main() {
    try {
        // Example 1: Pairs Trading
        PairsTrader pairsTrader(2.0);
        std::vector<double> asset1 = {100, 102, 101, 98, 97, 99};
        std::vector<double> asset2 = {99, 101, 100, 97, 96, 98};
        pairsTrader.loadData(asset1, asset2);
        pairsTrader.generateSignals();
        double pairsFinalCapital = pairsTrader.backtest(10000.0);
        std::cout << "Pairs Trading Final Capital: $" << pairsFinalCapital << std::endl;

        // Example 2: Mean-Reversion Strategy
        MeanReversionStrategy meanReversion(2, 4);
        std::vector<double> pricesMR = {100, 101, 102, 99, 97, 96, 98};
        meanReversion.loadData(pricesMR);
        meanReversion.generateSignals();
        double meanReversionFinalCapital = meanReversion.backtest(10000.0);
        std::cout << "Mean Reversion Final Capital: $" << meanReversionFinalCapital << std::endl;

        // Example 3: Momentum Strategy
        MomentumStrategy momentum(2);
        std::vector<double> pricesMomentum = {100, 102, 105, 110, 115, 120, 125};
        momentum.loadData(pricesMomentum);
        momentum.generateSignals();
        double momentumFinalCapital = momentum.backtest(10000.0);
        std::cout << "Momentum Final Capital: $" << momentumFinalCapital << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}

