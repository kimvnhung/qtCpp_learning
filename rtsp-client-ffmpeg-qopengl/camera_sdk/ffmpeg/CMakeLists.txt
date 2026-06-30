cmake_minimum_required(VERSION 3.25)
project(camera_ffmpeg LANGUAGES CXX)

add_library(camera_ffmpeg INTERFACE)

target_include_directories(camera_ffmpeg INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(camera_ffmpeg INTERFACE cxx_std_20)
