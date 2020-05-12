#include <stdio.h>
#include "../main.h"
#include "../../../../../../../usr/include/c++/7/fstream"
#include "../../../../../../../usr/include/c++/7/chrono"
#include "../../../../../../../usr/include/c++/7/iostream"

unsigned int Fib( unsigned int n ) { 
    if ( n < 2 ) return 1; 

    return Fib(n-1) + Fib(n-2); 
}

int main() {
    //Run for a number of repeat readings
    for (int run = 1; run <= REPEAT_READINGS; run++) {
        //Names and creates file to write results of current test run to
        std::string fileName = "../../data/fib_seq_" + std::to_string(run) + ".csv";
        std::ofstream outputFile(fileName);

        //Run fib(n) for n = 0 to n = MAX_FIB
        for (int n = 0; n <= MAX_FIB; n++) {
            //Calculate fib and record time taken
            auto startTime = std::chrono::high_resolution_clock::now();
            unsigned long int result = Fib(n);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            //Write to file and print
            outputFile << n << ", " << duration.count() << std::endl;
            std::cout << "Fib(" << n << ") = " << result << ", " << duration.count() << "ms" << std::endl;
        }
    }
}
