#include "lab3.h"
#include "utils.h"
#include <thread>
#include <iostream>

namespace
{
    void MinVectoRows(const TVector &lhs, TVector &result, int firstRow, int lastRow, int iterator)
    {
        int min1;
        min1 = lhs[firstRow];
        for (int j = firstRow; j < lastRow; ++j)
        {
            if (min1 > lhs[j])
            {
                min1 = lhs[j];
            }
        }
        result[iterator] = min1;
    }
}

int MinVector(const TVector &lhs, int threadCount)
{
    int min;
    int actualThreads = std::min(threadCount, isize(lhs));
    TVector result(actualThreads);
    if (threadCount > 1)
    {
        int iterator = 0;
        std::vector<std::thread> threads;
        int rowsPerThread = isize(lhs) / actualThreads;
        for (int i = 0; i < isize(lhs); i += rowsPerThread)
        {
            if (i + rowsPerThread >= isize(result))
            {
                threads.emplace_back(MinVectoRows, std::ref(lhs), std::ref(result), i, isize(lhs), iterator);
            }
            else
            {
                threads.emplace_back(MinVectoRows, std::ref(lhs), std::ref(result), i, i + rowsPerThread - 1, iterator);
            }
            ++iterator;
        }
        for (auto &thread : threads)
        {
            thread.join();
        }
    }
    else
    {
        MinVectoRows(lhs, result, 0, isize(lhs), 0);
    }
    min = result[0];
    for (int j = 0; j < isize(result); ++j)
    {
        if (min > result[j])
        {
            min = result[j];
        }
    }
    return min;
}
