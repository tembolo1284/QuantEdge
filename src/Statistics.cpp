#include "Statistics.hpp"

double Statistics::mean(const std::vector<double>& data) {
    double sum = 0.0;
    for (double value : data) sum += value;
    return sum / data.size();
}

double Statistics::stddev(const std::vector<double>& data) {
    double m = mean(data);
    double sum = 0.0;
    for (double value : data) sum += (value - m) * (value - m);
    return std::sqrt(sum / (data.size() - 1));
}

double Statistics::correlation(const std::vector<double>& data1, const std::vector<double>& data2) {
    double mean1 = mean(data1);
    double mean2 = mean(data2);
    double numerator = 0.0, denom1 = 0.0, denom2 = 0.0;

    for (size_t i = 0; i < data1.size(); ++i) {
        numerator += (data1[i] - mean1) * (data2[i] - mean2);
        denom1 += (data1[i] - mean1) * (data1[i] - mean1);
        denom2 += (data2[i] - mean2) * (data2[i] - mean2);
    }
    return numerator / std::sqrt(denom1 * denom2);
}

double Statistics::zScore(double value, double mean, double stddev) {
    return (value - mean) / stddev;
}

