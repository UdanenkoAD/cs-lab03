#include "histogram.h"
#include <vector>

using namespace std;


void find_minmax(struct Input t, double& min, double& max){
    if (t.numbers.size() == 0)
        return;
    min = t.numbers[0];
    max = t.numbers[0];
    for (double number : t.numbers) {
        if (number < min) {
            min = number;
        }
        if (number > max) {
            max = number;
        }
    }
        return ;
}
