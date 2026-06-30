
cmake_minimum_required(VERSION 3.25)
project(camera_ffmpeg LANGUAGES CXX)

set(ffmpeg_sources
    include/ffmpeg/FFmpegDecoder.h
    src/FFmpegDecoder.cpp
)

add_library(camera_ffmpeg SHARED ${ffmpeg_sources})

target_include_directories(camera_ffmpeg
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

message(STATUS "FFMPEG_LIBS: ${FFMPEG_LIBS}")
target_link_libraries(camera_ffmpeg
    PUBLIC
        camera_core
        ${FFMPEG_LIBS}
)

