#ifndef MOMENTUM_STRATEGY_HPP
#define MOMENTUM_STRATEGY_HPP

#include "StrategyBase.hpp"
#include <vector>

class MomentumStrategy : public StrategyBase {
private:
    std::vector<double> prices;
    std::vector<int> signals; // -1: sell, 1: buy, 0: hold
    int momentumPeriod;

public:
    MomentumStrategy(int momentumPeriod);
    void loadData(const std::vector<double>& prices) override;
    void generateSignals() override;
    double backtest(double initialCapital) override;
};

#endif // MOMENTUM_STRATEGY_HPP

