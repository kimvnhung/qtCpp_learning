#include "static_c.h"

#include "static_lib.h"

int StaticLib::add(int a, int b)
{
    return addFromStatic(a, b);
}

void StaticLib::printName()
{
    name();
}
