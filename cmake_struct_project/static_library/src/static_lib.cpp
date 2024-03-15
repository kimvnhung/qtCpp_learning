#include "static_lib.h"

#include <iostream>

void name()
{
    std::cout << "Hello from static lib" << std::endl;
}

int addFromStaticLib(int a, int b)
{
    return a + b;
}