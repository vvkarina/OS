#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include <stdio.h>
#include <array>
#include <stdlib.h>
#include <memory>
#include "parent.h"
#include <vector>

#include <iostream>
#include <fstream>
#include <sys/wait.h>

TEST(FirstLabTests, SimpleTest) {

   
    std::vector<std::string> inputIN {
            "TThE quick browNNN        ",
            "f0x jumps .OvEr. the l@zy dog"

     };

    std::vector<std::string> expectedOutput {
            "tthe_quick_brownnn________",
            "f0x_jumps_.over._the_l@zy_dog"

    };
       std::vector <std::string> output = ParentRoutine(inputIN);
         EXPECT_EQ(output, expectedOutput);
         

}