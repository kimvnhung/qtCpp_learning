cmake_minimum_required(VERSION 3.18)
cmake_policy(VERSION 3.18)

project(libvlc_usage)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets OpenGL OpenGLWidgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets OpenGL OpenGLWidgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE RESOURCES "src/*.qrc")
file(GLOB_RECURSE FORMS "src/*.ui")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES} ${FORMS})