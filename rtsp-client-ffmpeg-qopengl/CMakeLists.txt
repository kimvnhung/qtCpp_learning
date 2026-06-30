cmake_minimum_required(VERSION 3.14)

project(rtsp-client-ffmpeg-qopengl LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Quick Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Quick Qml)

# 3rdparty: glog fmt
set(BUILD_SHARED_LIBS OFF CACHE BOOL "build static libraries.")
find_package(fmt REQUIRED)
find_package(PkgConfig REQUIRED)

# Prefer pkg-config based FFmpeg discovery to ensure we link all required
# components (libavformat, libavcodec, libswscale, libavutil) for FFmpeg 7.x
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(SWSCALE REQUIRED libswscale)
pkg_check_modules(AVUTIL REQUIRED libavutil)

set(FFMPEG_INCLUDES ${AVFORMAT_INCLUDE_DIRS} ${AVCODEC_INCLUDE_DIRS} ${SWSCALE_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS})
set(FFMPEG_LIBS ${AVFORMAT_LIBRARIES} ${AVCODEC_LIBRARIES} ${SWSCALE_LIBRARIES} ${AVUTIL_LIBRARIES})

# Build SDK subproject and link against its libraries
add_subdirectory(camera_sdk)
