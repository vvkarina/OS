#include "lab3.h"
#include "utils.h"
#include <pthread.h>
#include <iostream>

pthread_mutex_t mutex;

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
        pthread_mutex_lock(&mutex);
        result[iterator] = min1;
        pthread_mutex_unlock(&mutex);
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
    pthread_mutex_init(&mutex, nullptr);
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
            if (i + rowsPerThread >= isize(result))
            {
                tokens[iterator].lhs = &lhs;
                tokens[iterator].result = &result;
                tokens[iterator].firstRow = i;
                tokens[iterator].lastRow = isize(lhs);
                tokens[iterator].iterator = iterator;
                pthread_create(&threads[iterator], nullptr, &MinVectorRowsRoutine, &tokens[iterator]);
            }
            else
            {
                tokens[iterator].lhs = &lhs;
                tokens[iterator].result = &result;
                tokens[iterator].firstRow = i;
                tokens[iterator].lastRow = (i + rowsPerThread - 1);
                tokens[iterator].iterator = iterator;
                pthread_create(&threads[iterator], nullptr, &MinVectorRowsRoutine, &tokens[iterator]);
            }
            ++iterator;
        }
        for (int i = 0; i < actualThreads; i++)
        {
            pthread_join(threads[i], nullptr);
        }
        pthread_mutex_destroy(&mutex);
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
