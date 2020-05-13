# CS4204-P2-DC
Second practical for CS4204 Concurrency and Multi-core Architectures module

Compilation/Execution Instructions

This practical was implemented using C++ 17.

Compilation

1.  Open a terminal window in the ’CS4204-P2-DC/src’ directory

2.  Run the command ’make’ to run the Makefile

Execution

1.  Open a terminal window in the ’CS4204-P2-DC/src’ directory

2.  Run a command, such as the following, to execute the program:

./dac -p <num cores to use> -f -m -q 

-p, specifies the number of CPU cores to use

-f, specifies that the Fibonacci benchmark should be run

-m, specifies that the merge sort benchmark should be run

-q, specifies that the quick sort benchmark should be run

For example:

./dac -p 4 -f -m -q

The above command runs the Fibonacci, merge sort, and quick sort benchmarks using 4 CPU cores.

The results of each benchmark are written to a file in the ’CS4204-P2-DC/data’ directory.

The  data  files  produced  are  named  using  the  benchmark  which  was  run,  followed  bythe number of CPU cores used, followed by the current repeat reading performed.

For example, the files produced by running the above command are as follows:

    fib_4_1.csv, 
    fib_4_2.csv, 
    fib_4_3.csv,
    merge_4_1.csv, 
    merge_4_2.csv, 
    merge_4_3.csv,
    qsort_4_1.csv, 
    qsort_4_2.csv, 
    qsort_4_3.csv
