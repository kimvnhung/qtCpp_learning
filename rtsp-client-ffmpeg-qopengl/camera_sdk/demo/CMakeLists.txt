cmake_minimum_required(VERSION 3.25)
project(camera_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src
    "*.cpp"
    "*.h"
)

file(GLOB_RECURSE QML_FILES RELATIVE
    ${CMAKE_CURRENT_SOURCE_DIR}
    "*.qml"
)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI CameraDemo
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CameraDemo"
    RESOURCE_PREFIX /qt/qml
    QML_FILES
        ${QML_FILES}
    SOURCES
        ${SOURCES}
)
target_include_directories(camera_demo
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(camera_demo
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        camera_core
        camera_ffmpeg
        camera_renderer
        ${FFMPEG_LIBS}
)

install(TARGETS camera_demo RUNTIME DESTINATION bin)
