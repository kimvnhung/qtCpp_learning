cmake_minimum_required(VERSION 3.5)

project(OpenGLBegin VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets Gui OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets Gui OpenGL)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        qglbegin.h qglbegin.cpp
        texturewidget.h texturewidget.cpp
        qopenglskyboxwidget.h qopenglskyboxwidget.cpp
        data_type.h
        videotexturewidget.h videotexturewidget.cpp
        glvideowidget.h glvideowidget.cpp
        ffmpegplayer.h ffmpegplayer.cpp
        framequeue.h framequeue.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OpenGLBegin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        assets.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OpenGLBegin APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(OpenGLBegin SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OpenGLBegin
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(
    FFMPEG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../ffmpeg"
)
set(FFMPEG_INCLUDES "${FFMPEG_DIR}/include")

target_include_directories(OpenGLBegin
    PRIVATE
        ${FFMPEG_INCLUDES}
)

file(GLOB_RECURSE DLL_FILES "${FFMPEG_DIR}/.*dll")
foreach(DLL_FILE ${DLL_FILES})
    configure_file(${DLL_FILE} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()

file(GLOB FFMPEG_LIBS "${FFMPEG_DIR}/lib/*.lib")


target_link_libraries(OpenGLBegin
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::OpenGL
        ${FFMPEG_LIBS}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.OpenGLBegin)
endif()
set_target_properties(OpenGLBegin PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS OpenGLBegin
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OpenGLBegin)
endif()
