#include "PairsTrader.hpp"
#include "MeanReversionStrategy.hpp"
#include "MomentumStrategy.hpp"
#include <gtest/gtest.h>
#include <vector>

// Test Pairs Trading
TEST(PairsTraderTest, BasicFunctionality) {
    PairsTrader pairsTrader(2.0);
    std::vector<double> asset1 = {100, 102, 101, 98, 97, 99};
    std::vector<double> asset2 = {99, 101, 100, 97, 96, 98};
    pairsTrader.loadData(asset1, asset2);
    pairsTrader.generateSignals();
    double finalCapital = pairsTrader.backtest(10000.0);
    EXPECT_NEAR(finalCapital, 10010.0, 0.01); // Expected final capital
}

// Test Mean-Reversion Strategy
TEST(MeanReversionStrategyTest, BasicFunctionality) {
    MeanReversionStrategy meanReversion(2, 4);
    std::vector<double> prices = {100, 101, 102, 99, 97, 96, 98};
    meanReversion.loadData(prices);
    meanReversion.generateSignals();
    double finalCapital = meanReversion.backtest(10000.0);
    EXPECT_GT(finalCapital, 10000.0); // Expect positive returns
}

// Test Momentum Strategy
TEST(MomentumStrategyTest, BasicFunctionality) {
    MomentumStrategy momentum(2);
    std::vector<double> prices = {100, 102, 105, 110, 115, 120, 125};
    momentum.loadData(prices);
    momentum.generateSignals();
    double finalCapital = momentum.backtest(10000.0);
    EXPECT_GT(finalCapital, 10000.0); // Expect positive returns
}

// Main function for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

