#include <iostream>
#include "FibSkeleton.h"

int main() {
    int n;
    std::cout << "Please enter an integer value: ";
    std::cin >> n;
    FibSkeleton fibSkeleton;
    std::cout << "Fib(" << n << ") = " << fibSkeleton.solve(n) << std::endl;
    return 0;
}