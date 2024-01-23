cmake_minimum_required(VERSION 3.5)

project(rtsp_client_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED
    COMPONENTS
        Widgets
        Core
        Gui
        Network
        MultimediaWidgets
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS
        Widgets
        Core
        Gui
        Network
        MultimediaWidgets
)

if (LINUX)
    find_library(AVCODEC_LIBRARY avcodec)
else()
    set(
        LIB_AVCODEC_INCLUDE_DIR
            ${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/include
    )
endif()

# Set the source directory to the root directory containing CMakeLists.txt
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Set the include directory

# Find all .cpp files in the source directory
set(
    PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        streamclientstate.h
        streamclientstate.cpp
        dummysink.h
        dummysink.cpp
        videowidget.h
        videowidget.cpp
        rtspclientmanager.h
        rtspclientmanager.cpp
        framequeue.h
        framequeue.cpp
        common.h
        rtspconsumefunction.h
        rtspconsumefunction.cpp
        rtspclientimpl.h
        rtspclientimpl.cpp
        framelistener.h
        framedecoder.h
        framedecoder.cpp

        videodecoder.h
        videodecoder.cpp
        videoplayer.h
        videoplayer.cpp

)

# Print the list of source files (optional)
# message("Source Files: ${SOURCE_FILES}")
message("Include Dir: ${INCLUDE_DIRS}")

# Add the include directory

add_subdirectory(live555)
add_subdirectory(libde265)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(rtsp_client_demo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET rtsp_client_demo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(rtsp_client_demo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(rtsp_client_demo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libde265)

if(LINUX)
    target_link_libraries(rtsp_client_demo
        PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::Network
            Qt${QT_VERSION_MAJOR}::MultimediaWidgets
            live555
            de265
            AVCODEC_LIBRARY
    )
else()
    # Include the include directory
    target_include_directories(rtsp_client_demo PRIVATE ${LIB_AVCODEC_INCLUDE_DIR})


    # Copy all DLL files to the build directory during the build process
    file(GLOB DLL_FILES "${DLL_DIR}/*.dll")
    foreach(DLL_FILE ${DLL_FILES})
        configure_file(${DLL_FILE} ${CMAKE_BINARY_DIR} COPYONLY)
    endforeach()

    # Link against the DLL (assuming Windows)
    target_link_libraries(rtsp_client_demo
        PRIVATE
            Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::Network
            Qt${QT_VERSION_MAJOR}::MultimediaWidgets
            live555
            de265
            DLL_FILES
    )
endif()



include_directories(${libde265_BINARY_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.rtsp_client_demo)
endif()
set_target_properties(rtsp_client_demo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS rtsp_client_demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(rtsp_client_demo)
endif()
