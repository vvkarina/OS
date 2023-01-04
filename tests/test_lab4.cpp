#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include "parent.h"
#include <vector>

TEST(FirstLabTests, SimpleTest)
{
    std::vector<std::string> inputIN{
        "TThE quick browNNN        ",
        "          ",
        "",
        "f0x jumps .OvEr. the l@zy dog"};
    std::vector<std::string> expectedOutput{
        "tthe_quick_brownnn________",
        "__________",
        "",
        "f0x_jumps_.over._the_l@zy_dog"};
    std::vector<std::string> output = ParentRoutine(inputIN);
    EXPECT_EQ(output, expectedOutput);
}