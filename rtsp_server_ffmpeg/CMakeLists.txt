cmake_minimum_required(VERSION 3.14)

project(rtsp_server_ffmpeg LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(${PROJECT_NAME}
  main.cpp
  rtspserver.h rtspserver.cpp
)

# Include FFMPEG
include(../ffmpeg/ffmpeg.cmake)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        ${FFMPEG_LIBS}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${FFMPEG_INCLUDES}
)

include(GNUInstallDirs)
install(TARGETS rtsp_server_ffmpeg
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
