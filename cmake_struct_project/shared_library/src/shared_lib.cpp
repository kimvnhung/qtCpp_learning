#include "shared_lib.h"

#include <iostream>
void name()
{
    std::cout << "Hello from shared lib" << std::endl;
}

int multipleFromShared(int a, int b)
{
    return a * b;
}