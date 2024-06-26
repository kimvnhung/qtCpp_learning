cmake_minimum_required(VERSION 3.5)

project(nx_time_line VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets QuickWidgets Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets QuickWidgets Gui)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        bubbletooltip.h bubbletooltip.cpp
        demoquickwidget.h demoquickwidget.cpp
        sampleview.h sampleview.cpp
        models/sample.h models/sample.cpp
        bubbletooltip2.h bubbletooltip2.cpp
        singleton.h singleton.cpp
        color_substitutions.h
        timemarker.h timemarker.cpp
        formatter.h formatter.cpp
        qmlwidget.h qmlwidget.cpp
        timerplayback.h timerplayback.cpp
        models/timerplaybackmodel.h models/timerplaybackmodel.cpp
        models/ruleline.h models/ruleline.cpp
        models/linedata.h models/linedata.cpp
        models/ruler.h models/ruler.cpp
        models/timestep.h models/timestep.cpp
        timerplayback2.h timerplayback2.cpp
        models/rulercontext.h models/rulercontext.cpp
        models/basecontextobject.h models/basecontextobject.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(nx_time_line
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}



    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET nx_time_line APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(nx_time_line SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(nx_time_line
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(nx_time_line
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::QuickWidgets
)

# find_package(FelgoLive REQUIRED)
# target_link_libraries(nx_time_line PRIVATE FelgoLive)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.nx_time_line)
endif()
set_target_properties(nx_time_line PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS nx_time_line
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(nx_time_line)
endif()
