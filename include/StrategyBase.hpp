#ifndef STRATEGY_BASE_HPP
#define STRATEGY_BASE_HPP

#include <vector>
#include <string>

class StrategyBase {
public:
    virtual void loadData(const std::vector<double>& prices) = 0;
    virtual void generateSignals() = 0;
    virtual double backtest(double initialCapital) = 0;
    virtual ~StrategyBase() = default;
};

#endif // STRATEGY_BASE_HPP

