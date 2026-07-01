cmake_minimum_required(VERSION 3.14)

project(algorithms LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE ALGORITHM_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/*.cpp
    src/*.h
)

add_executable(algorithms main.cpp ${ALGORITHM_SOURCES})

target_link_libraries(algorithms
    PUBLIC
        Qt6::Core
        Qt6::Gui
)

target_include_directories(algorithms
    PUBLIC
        ${INCLUDE_DIR}
)