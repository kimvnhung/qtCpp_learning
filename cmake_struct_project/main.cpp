#include <iostream>

#include <normal_library/my_library.h>

int main() {
    MyClass myClass("hello");
    StaticLib lib;
    SharedLib lib2;
    myClass.testLink(lib);
    myClass.testLink(lib2);
    int i;
    std::cin>>i;
    return 0;
}
