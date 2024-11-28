#ifndef MEAN_REVERSION_STRATEGY_HPP
#define MEAN_REVERSION_STRATEGY_HPP

#include "StrategyBase.hpp"
#include <vector>

class MeanReversionStrategy : public StrategyBase {
private:
    std::vector<double> prices;
    std::vector<int> signals; // -1: short, 1: long, 0: no position
    int shortWindow;
    int longWindow;

public:
    MeanReversionStrategy(int shortWindow, int longWindow);
    void loadData(const std::vector<double>& prices) override;
    void generateSignals() override;
    double backtest(double initialCapital) override;
};

#endif // MEAN_REVERSION_STRATEGY_HPP

