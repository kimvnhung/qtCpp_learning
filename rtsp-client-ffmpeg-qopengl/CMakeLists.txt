cmake_minimum_required(VERSION 3.14)

project(rtsp-client-ffmpeg-qopengl LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

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

add_executable(${PROJECT_NAME}
  main.cpp
  defer.h
)

set(FMT_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/fmt/include")
# set(GLOG_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/glog/src")

# Include the include directory
target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDES} ${FMT_INCLUDES})

# Link against the DLL (assuming Windows)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
    PUBLIC
        ${FFMPEG_LIBS}
        ${OpenCV_LIBS}
        # glog::glog
        fmt::fmt
)

include(GNUInstallDirs)
install(TARGETS rtsp-client-ffmpeg-qopengl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
