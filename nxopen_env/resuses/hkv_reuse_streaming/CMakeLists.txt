# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project(hkv_reuse_streaming)

# Find the Qt5 package
find_package(Qt5
    COMPONENTS
        Widgets
        Core
    REQUIRED
)

# Add your source files
set(
    SOURCES
        main.cpp
        mainwindow.cpp
        resourcecontainer.cpp
)

set(
    HEADERS
        mainwindow.h
        resourcecontainer.h
)

# Create an executable target
add_executable(hkv_reuse_streaming ${SOURCES} ${HEADERS})

# Link the target with the Qt5 libraries
target_link_libraries(hkv_reuse_streaming 
    PRIVATE 
        Qt5::Core
        Qt5::Widgets
        Qt5::Qml
)
