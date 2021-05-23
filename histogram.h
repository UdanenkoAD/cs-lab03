#pragma once
#include <vector>

using namespace std;

struct Input {
    vector<double> numbers;
    size_t bin_count;
};

void find_minmax(struct Input t, double& min, double& max);

