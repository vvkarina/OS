#include "lab3.h"
#include "utils.h"
#include <thread>
#include <iostream>

namespace
{
    void MinVectoRows(const TVector &lhs, TVector &result, unsigned int firstRow, unsigned int lastRow, unsigned int iterator)
    {
        unsigned int min1;
        min1 = lhs[firstRow];
        for (unsigned int j = firstRow; j < lastRow; ++j)
        {
            if (min1 > lhs[j])
            {
                min1 = lhs[j];
            }
        }
        result[iterator] = min1;
    }
}

int MinVector(const TVector &lhs, unsigned int threadCount)
{
    unsigned int min;
    unsigned int sizev;
    sizev = lhs.size();
    unsigned int actualThreads = std::min(threadCount, sizev);
    TVector result(actualThreads);
    if (threadCount > 1)
    {
        unsigned int iterator = 0;
        std::vector<std::thread> threads;
        unsigned int rowsPerThread = sizev / actualThreads;
        for (unsigned int i = 0; i < sizev; i += rowsPerThread)
        {
            if (i + rowsPerThread >= result.size())
            {
                threads.emplace_back(MinVectoRows, std::ref(lhs), std::ref(result), i, sizev, iterator);
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
        MinVectoRows(lhs, result, 0, sizev, 0);
    }
    min = result[0];
    unsigned int end = result.size();
    for (unsigned int j = 0; j < end; ++j)
    {
        if (min > result[j])
        {
            min = result[j];
        }
    }
    return min;
}
