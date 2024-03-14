#include <my_library/my_library.h>

MyClass::MyClass(const char* name):
    m_name(name)
{

}

std::string MyClass::getName()
{
    return std::string(m_name);
}
