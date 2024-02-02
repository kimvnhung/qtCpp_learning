cmake_minimum_required(VERSION 3.14)

project(rtsp_server_qt_demo LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(rtsp_server_qt_demo
  main.cpp
  announceURL.hh
  announceURL.cpp
)
add_subdirectory(live555)

if(WIN32)
    set(FFMPEG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg)
    set(FFMPEG_INCLUDE ${FFMPEG_DIR}/include)
    set(FFMPEG_DLL ${FFMPEG_DIR}/bin)
    set(FFMPEG_LIB ${FFMPEG_DIR}/lib)

    # Include the include directory
    target_include_directories(rtsp_server_qt_demo PRIVATE ${FFMPEG_INCLUDE})


    # Copy all DLL files to the build directory during the build process
    file(GLOB DLL_FILES "${DLL_DIR}/*.dll")
    foreach(DLL_FILE ${DLL_FILES})
        configure_file(${DLL_FILE} ${CMAKE_BINARY_DIR} COPYONLY)
    endforeach()

    file(GLOB FFMPEG_LIBS "${FFMPEG_LIB}/*.lib")

    # Link against the DLL (assuming Windows)
    target_link_libraries(rtsp_server_qt_demo
        PRIVATE
            Qt${QT_VERSION_MAJOR}::Core
            live555
            ${FFMPEG_LIBS}
    )
endif()



include(GNUInstallDirs)
install(TARGETS rtsp_server_qt_demo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
