#include "lab3.h"
#include "utils.h"
#include <pthread.h>
#include <iostream>

namespace
{
    void MinVectorRows(const TVector &lhs, TVector &result, int firstRow, int lastRow, int iterator)
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

void *MinVectorRowsRoutine(void *arg)
{
    auto *token = (TThreadToken *)arg;
    MinVectorRows(*token->lhs, *token->result, token->firstRow, token->lastRow, token->iterator);
    return nullptr;
}

int MinVector(const TVector &lhs, int threadCount)
{
    int min;
    int actualThreads = std::min(threadCount, isize(lhs));
    TVector result(actualThreads);
    if (threadCount > 1)
    {
        int iterator = 0;
        int rowsPerThread = isize(lhs) / actualThreads;
        std::vector<pthread_t> threads(actualThreads);
        std::vector<TThreadToken> tokens(actualThreads);

        for (int i = 0; i < isize(lhs); i += rowsPerThread)
        {
            tokens[iterator].lhs = &lhs;
            tokens[iterator].result = &result;
            tokens[iterator].firstRow = i;
            tokens[iterator].iterator = iterator;
            if (i + rowsPerThread >= isize(result))
            {
                tokens[iterator].lastRow = isize(lhs);
                pthread_create(&threads[iterator], nullptr, &MinVectorRowsRoutine, &tokens[iterator]);
            }
            else
            {
                tokens[iterator].lastRow = (i + rowsPerThread - 1);
                pthread_create(&threads[iterator], nullptr, &MinVectorRowsRoutine, &tokens[iterator]);
            }
            ++iterator;
        }
        for (int i = 0; i < actualThreads; i++)
        {
            pthread_join(threads[i], nullptr);
        }
    }
    else
    {
        MinVectorRows(lhs, result, 0, isize(lhs), 0);
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
