cmake_minimum_required(VERSION 3.14)
project(CS4204_P2_DC)

set(CMAKE_CXX_STANDARD 17)

add_executable(CS4204_P2_DC main.cpp Task.cpp Task.h DCSkeleton.h pfib.h mergesort.h qsort.h utility.h FibWorker.cpp FibWorker.h Worker.cpp Worker.h)