#include <normal_library/my_library.h>

MyClass::MyClass(const char* name):
    m_name(name)
{

}

void MyClass::testLink(StaticLib lib)
{
    lib.printName();
    std::cout << "print add static lib "<<lib.add(1,1)<<std::endl;
}

void MyClass::testLink(SharedLib lib)
{
    lib.printName();
    std::cout << "print add shared lib "<<lib.multiple(1,2)<<std::endl;
}
