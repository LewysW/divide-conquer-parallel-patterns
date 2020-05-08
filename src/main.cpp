#include <iostream>
#include "FibWorker.h"

int main() {
    int n;
    std::cout << "Please enter an integer value: ";
    std::cin >> n;
    FibWorker fibWorker;
    std::cout << "Fib(" << n << ") = " << fibWorker.solve(n) << std::endl;
    return 0;
}