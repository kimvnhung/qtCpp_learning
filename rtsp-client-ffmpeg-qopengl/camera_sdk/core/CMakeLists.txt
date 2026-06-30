cmake_minimum_required(VERSION 3.25)
project(camera_core LANGUAGES CXX)

add_library(camera_core STATIC
    src/VideoFrame.cpp
)

target_include_directories(camera_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(camera_core PUBLIC cxx_std_20)
