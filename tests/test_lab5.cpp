#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <dlfcn.h>
#include "lib.h"

TEST(Lab5Test, StaticOneTest)
{
    const float expectE = 2.71;
    const auto inputParameterE = {500, 300, 200};

    const std::vector<std::vector<float>> inputParameter = {
        {1, 50},
        {5, 6700},
        {77, 956}};
    const std::vector<float> answer = {15, 861, 141};

    for (const auto &elem : inputParameterE)
    {
        auto eOut = E(elem);
        EXPECT_NEAR(eOut, expectE, 0.1);
    }

    for (size_t i = 0; i < inputParameter.size(); ++i)
    {
        float a = inputParameter[i][0];
        float b = inputParameter[i][1];
        auto sq = PrimeCount(a, b);
        EXPECT_NEAR(sq, answer[i], 1);
    }
}