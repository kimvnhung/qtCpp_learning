cmake_minimum_required(VERSION 3.14)

project(rtsp_checker LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(rtsp_checker
  main.cpp
)

include(../ffmpeg/ffmpeg.cmake)

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDES})
target_link_libraries(rtsp_checker
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    ${FFMPEG_LIBS}
)

include(GNUInstallDirs)
install(TARGETS rtsp_checker
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
