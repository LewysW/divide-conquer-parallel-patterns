cmake_minimum_required(VERSION 3.14)
project(CS4204_P2_DC)

set(CMAKE_CXX_STANDARD 17)

add_executable(CS4204_P2_DC main.cpp Task.cpp Task.h DCSkeleton.h utility.h FibWorker.h Worker.cpp Worker.h MergeSortWorker.h QuickSortWorker.h List.h)

target_link_libraries(${PROJECT_NAME} pthread)