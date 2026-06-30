cmake_minimum_required(VERSION 3.25)
project(camera_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml)

qt_add_executable(camera_demo
    src/main.cpp
    src/DemoBridge.cpp
)

qt_add_qml_module(camera_demo
    URI CameraSdkDemo
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

target_include_directories(camera_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(camera_demo
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        camera_core
        camera_ffmpeg
        camera_renderer
)

install(TARGETS camera_demo RUNTIME DESTINATION bin)
