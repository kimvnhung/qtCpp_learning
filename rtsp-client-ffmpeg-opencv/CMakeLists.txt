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



if(WIN32)
    # Set OpenCV_DIR to Qt_SOURCE/opencv/build
    set(OpenCV_DIR "C:/Qt/opencv/build")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
  main.cpp
  defer.h
  # logger.h
)


include(../ffmpeg/ffmpeg.cmake)

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
install(TARGETS rtsp-client-ffmpeg-opencv
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
