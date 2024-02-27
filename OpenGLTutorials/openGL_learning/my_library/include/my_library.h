#include <iostream>

class MyClass {
public:
    MyClass(const char* name);

    std::string getName();
private:
    const char* m_name;
};
