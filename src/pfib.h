#ifndef CS4204_P2_DC_PFIB_H
#define CS4204_P2_DC_PFIB_H

#include <cstdio>
#include "DCSkeleton.h"

unsigned int Fib( unsigned int n ) {
    if ( n < 2 ) return 1;

    return Fib(n-1) + Fib(n-2);
}
#endif //CS4204_P2_DC_PFIB_H
