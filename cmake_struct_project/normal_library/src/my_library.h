#include <iostream>

#include <static_library/static_c.h>
#include <shared_library/shared_c.h>

class MyClass {
public:
    MyClass(const char* name);

    void testLink(StaticLib lib);
    void testLink(SharedLib lib);
private:
    const char* m_name;
};
