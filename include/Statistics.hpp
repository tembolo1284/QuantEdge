#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>
#include <cmath>

class Statistics {
public:
    static double mean(const std::vector<double>& data);
    static double stddev(const std::vector<double>& data);
    static double correlation(const std::vector<double>& data1, const std::vector<double>& data2);
    static double zScore(double value, double mean, double stddev);
};

#endif // STATISTICS_HPP

