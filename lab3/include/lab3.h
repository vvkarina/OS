#ifndef OS_LABS_LAB3_H
#define OS_LABS_LAB3_H
#include <vector>

using TVector = std::vector<int>;
int MinVector(const TVector &lhs, int threadCount);

struct TThreadToken {
    const TVector* lhs;
    TVector* result;
    int firstRow;
    int lastRow;
    int iterator;
};




#endif // OS_LABS_LAB3_H
