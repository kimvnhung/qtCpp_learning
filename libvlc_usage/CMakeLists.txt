cmake_minimum_required(VERSION 3.18)
cmake_policy(VERSION 3.18)

project(libvlc_usage LANGUAGES CXX)

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

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")


include_directories(${LIB_DIR}/libvlc/include)
include_directories(${LIB_DIR}/libvlcpp)

file(GLOB_RECURSE LIBVLC_LIBRARY "${LIB_DIR}/libvlc/lib/*.lib")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES} ${FORMS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Concurrent
        Qt${QT_VERSION_MAJOR}::Multimedia
        Qt${QT_VERSION_MAJOR}::MultimediaWidgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        Qt${QT_VERSION_MAJOR}::Network

    PUBLIC
        ${LIBVLC_LIBRARY}
)
