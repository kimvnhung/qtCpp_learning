cmake_minimum_required(VERSION 3.25)
project(camera_consumer LANGUAGES CXX)

file(GLOB_RECURSE consumer_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
    *.h
)

add_library(camera_consumer ${consumer_sources})

target_include_directories(camera_consumer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(camera_consumer
    PUBLIC
        camera_ffmpeg
        Qt6::Quick
        Qt6::Gui
        Qt6::Core
        Qt6::Multimedia
)

