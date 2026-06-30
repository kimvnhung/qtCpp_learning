cmake_minimum_required(VERSION 3.25)
project(camera_renderer LANGUAGES CXX)

add_library(camera_renderer INTERFACE)

target_include_directories(camera_renderer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(camera_renderer INTERFACE cxx_std_20)
