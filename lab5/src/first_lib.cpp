#include "lib.h"

int PrimeCount(int start, int finish)
{
    printf("PrimeCount with naive algoritm\n");
    if (start == 1)
    {
        ++start;
    }
    int prime_count = 0;
    int is_prime;
    for (; start <= finish; ++start)
    {
        is_prime = 1;
        for (int number = 2; number < start; ++number)
        {
            if (start % number == 0)
            {
                is_prime = 0;
                break;
            }
        }
        if (is_prime == 1)
        {
            ++prime_count;
        }
    }
    return prime_count;
}

float E(int argument)
{
    printf("Calculating a E with first wonderful limit\n");
    if (argument < 0)
    {
        return -1;
    }
    float e = 1.0;
    for (int index = 0; index < argument; ++index)
    {
        e *= 1 + 1 / (float)argument;
    }
    return e;
}
