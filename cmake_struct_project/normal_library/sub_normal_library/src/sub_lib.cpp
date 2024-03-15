#include "sub_lib.h"

#include <iostream>

void name()
{
    std::cout << "Hello From Sub lib" << std::endl;
}

int add(int a, int b)
{
    return a + b + 1;
}

int multiple(int a, int b)
{
    return a * b + 1;
}
