#include <iostream>
#include "my_library.h"  // Include the header file from the library

int main() {
    MyClass myclass("Hung");

    std::cout << "Hello, world! " <<myclass.getName() << std::endl;
    return 0;
}
