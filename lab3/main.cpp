#include "lab3.h"

#include <iostream>

int main()
{
    int m;
    int threadCount;
    std::cin >> m >> threadCount;
    if (threadCount == 0)
    {
        threadCount = 1;
    }
    TVector lhs(m);
    for (int i = 0; i < m; ++i)
    {
        std::cin >> lhs[i];
    }

    int min = MinVector(lhs, threadCount);
    std::cout << "Minimum:" << min;
    std::cout << '\n';
}
