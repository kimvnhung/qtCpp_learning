#include "shared_c.h"

#include "shared_lib.h"

void SharedLib::printName()
{
    name();
}

int SharedLib::multiple(int a,int b)
{
    return multipleFromShared(a,b);
}
