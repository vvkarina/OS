#include "parent.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

int main(void) {
    std::vector <std::string> input;

    std::string s;
    while (getline(std::cin, s)) {
        input.push_back(std::move(s));
    }
    std::vector <std::string> output = ParentRoutine(input);

    for (const auto &res : output){
        std::cout << res << std::endl;
    }
    return 0;
}