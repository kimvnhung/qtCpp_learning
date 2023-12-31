cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR) #< This version is bundled with MSVC 2019.

project(hkv_reuse_streaming VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_DEFAULT_MAJOR_VERSION 6)

message(STATUS "QT_DEFAULT_MAJOR_VERSION : " ${QT_DEFAULT_MAJOR_VERSION})
message(STATUS "QT_VERSION_MAJOR : " ${QT_VERSION_MAJOR})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Qml
        Quick
        QuickWidgets
)



qt_standard_project_setup()

qt_add_executable(hkv_reuse_streaming
    main.cpp
)

qt6_policy(
    SET QTP0001 NEW
)

qt_add_qml_module(hkv_reuse_streaming
    URI
        hkv_reuse_streaming
    VERSION
        1.0
    QML_FILES
        main.qml
    SOURCES
        mainwindow.cpp mainwindow.h
        resourcecontainer.h resourcecontainer.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(hkv_reuse_streaming PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.hkv_reuse_streaming
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(hkv_reuse_streaming
    PRIVATE
        Qt6::Quick
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickWidgets

)


include(GNUInstallDirs)
install(TARGETS hkv_reuse_streaming
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
