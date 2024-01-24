cmake_minimum_required(VERSION 3.14)

project(rtsp-client-ffmpeg-opencv LANGUAGES CXX)

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
set(WITH_GFLAGS OFF CACHE BOOL "Use gflags")
# add_subdirectory(glog EXCLUDE_FROM_ALL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)

add_executable(rtsp-client-ffmpeg-opencv
  main.cpp
  mediadecoder.h mediadecoder.cpp
  videoplayer.h videoplayer.cpp
  defer.h
  # logger.h
)


set(
    FFMPEG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg"
)

set(
    OPENCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv"
)

set(OPENCV_INCLUDES "${OPENCV_DIR}/include")
set(FFMPEG_INCLUDES "${FFMPEG_DIR}/include")
set(FMT_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/fmt/include")
# set(GLOG_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/glog/src")

# Include the include directory
target_include_directories(rtsp-client-ffmpeg-opencv PRIVATE ${OPENCV_INCLUDES} ${FFMPEG_INCLUDES} ${FMT_INCLUDES})


file(GLOB_RECURSE DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/.*dll")
foreach(DLL_FILE ${DLL_FILES})
    configure_file(${DLL_FILE} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()


file(GLOB FFMPEG_LIBS "${FFMPEG_DIR}/lib/*.lib")
file(GLOB OPENCV_LIBS "${OPENCV_DIR}/lib/*.lib")

# Link against the DLL (assuming Windows)
target_link_libraries(rtsp-client-ffmpeg-opencv
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        ${FFMPEG_LIBS}
        ${OPENCV_LIBS}
        # glog::glog
        fmt::fmt
)

include(GNUInstallDirs)
install(TARGETS rtsp-client-ffmpeg-opencv
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
