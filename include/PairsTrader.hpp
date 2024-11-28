#ifndef PAIRS_TRADER_HPP
#define PAIRS_TRADER_HPP

#include <vector>
#include <string>

class PairsTrader {
public:
    PairsTrader(double zThreshold);
    void loadData(const std::vector<double>& asset1, const std::vector<double>& asset2);
    void loadDataFromFiles(const std::string& file1, const std::string& file2);
    void generateSignals();
    double backtest(double initialCapital);

private:
    std::vector<double> asset1Prices;
    std::vector<double> asset2Prices;
    std::vector<int> signals; // -1: short spread, 1: long spread, 0: no position
    double zThreshold;
};

#endif // PAIRS_TRADER_HPP

