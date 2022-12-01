#include <gtest/gtest.h>

#include <lab3.h>
#include <utils.h>

#include <chrono>

namespace
{
    TVector GenerateMatrix(unsigned int m)
    {
        TVector result1(m);
        std::srand(std::time(nullptr));
        for (unsigned int i = 0; i < m; ++i)
        {
            result1[i] = std::rand() % 100 + 1;
        }
        return result1;
    }
}

TEST(ThirdLabTests, SingleThreadYieldsCorrectResults)
{
    EXPECT_EQ(MinVector(TVector{1}, 1), 1);
    EXPECT_EQ(MinVector(TVector{6, 2}, 1), 2);
    EXPECT_EQ(MinVector(TVector{1, 5, 3}, 1), 1);
}

TEST(ThirdLabTest, ThreadConfigurations)
{
    auto performTestForGivenSize = [](unsigned int m, unsigned int maxThreadCount)
    {
        auto m1 = GenerateMatrix(m);
        auto result = MinVector(m1, 1);
        for (unsigned int i = 2; i < maxThreadCount; ++i)
        {
            EXPECT_EQ(MinVector(m1, i), result);
        }
    };
    performTestForGivenSize(3, 3);
    performTestForGivenSize(100, 2);
    performTestForGivenSize(1000, 3);
    performTestForGivenSize(100000000, 4);
}

TEST(ThirdLabTest, PerfomanceTest)
{
    auto getAvgTime = [](unsigned int threadCount)
    {
        auto m1 = GenerateMatrix(400000000);
        constexpr int runsCount = 1;
        double avg = 0;
        for (int i = 0; i < runsCount; ++i)
        {
            auto begin = std::chrono::high_resolution_clock::now();
            MinVector(m1, threadCount);
            auto end = std::chrono::high_resolution_clock::now();
            avg += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        }
        return avg / runsCount;
    };
    auto singleThread = getAvgTime(1);
    auto multiThread = getAvgTime(4);
    std::cout << "Avg time for 1 thread: " << singleThread << '\n';
    std::cout << "Avg time for 4 threads: " << multiThread << '\n';
    EXPECT_GE(singleThread, multiThread);
}