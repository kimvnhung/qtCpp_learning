# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project(hkv_reuse_streaming)

# Find the Qt5 package
find_package(Qt5
    COMPONENTS
        Widgets
        Qml
        Quick
        Gui
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

set(
    QML_FILES
    main.qml
)

# Create an executable target
add_executable(hkv_reuse_streaming ${SOURCES} ${HEADERS} ${QML_FILES})

# Link the target with the Qt5 libraries
target_link_libraries(hkv_reuse_streaming 
    PRIVATE
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
        Qt5::Gui
        Qt5::Core
)

message(STATUS "CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})

# Include directories for headers
# target_include_directories(hkv_reuse_streaming PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Set the QML files as resources
# qt5_add_resources(QML_RESOURCES qml.qrc)
# target_sources(hkv_reuse_streaming PRIVATE ${QML_RESOURCES})
