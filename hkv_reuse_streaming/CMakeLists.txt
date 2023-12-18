cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR) #< This version is bundled with MSVC 2019.

set(open_source_root "${CMAKE_SOURCE_DIR}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(cmake/find_python.cmake)
find_package(PythonInterp)

include(cmake/open_source/defaults.cmake)
include(cmake/customization_package.cmake)
include(cmake/default_target.cmake)

set(targetDevice ${default_target_device} CACHE STRING
    "Target device. For the list see cmake/toolchains.")
if(NOT clangDir STREQUAL "")
    set(ENV{CLANG_DIR} ${clangDir})
endif()
include(cmake/open_source/toolchain_detection.cmake)
include(cmake/option.cmake)
nx_option(developerBuild
    "Disable this option for production builds"
    ON)

include(cmake/dependencies.cmake)
find_package(OpenSSL REQUIRED)
include(cmake/qt_utils.cmake)

project(hkv_reuse_streaming VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(hkv_reuse_streaming
    main.cpp
)

qt_add_qml_module(hkv_reuse_streaming
    URI hkv_reuse_streaming
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES mainwindow.cpp mainwindow.h
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
)


include(GNUInstallDirs)
install(TARGETS hkv_reuse_streaming
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
