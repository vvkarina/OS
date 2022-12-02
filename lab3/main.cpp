#include "../lab3/include/lab3.h"

#include <iostream>

int main() {
    int  m; 
    int threadCount;
    std::cin >> m >> threadCount;
    if (threadCount==0)
    {
        threadCount=1;
    }
    TVector lhs(m);
    auto readVector = [m](TVector& matrix) {
          for(int i = 0; i < m; ++i) {
                  std::cin >> matrix[i];
          }
   };

    readVector(lhs);

    int min = MinVector(lhs, threadCount);
    std::cout << "Minimum:"<<min;
    std::cout << '\n';
    
}
