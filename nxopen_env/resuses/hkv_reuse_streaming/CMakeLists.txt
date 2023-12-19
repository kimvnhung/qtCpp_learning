cmake_minimum_required(VERSION 3.5)

project(hkv_reuse_streaming LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# INPORTANT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

# Set the minimum required version of Qt

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5QuickWidgets)

if(WIN32)
    find_package(Qt5WinExtras)
endif()

# Create the executable
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

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include the necessary directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against the required Qt modules
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)


# Set up QML files
qt5_add_resources(QML_RESOURCES qml.qrc)

# Add the QML resources to the target
target_sources(${PROJECT_NAME} PRIVATE ${QML_RESOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Qml Qt5::Quick Qt5::QuickWidgets)
