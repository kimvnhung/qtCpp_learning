cmake_minimum_required(VERSION 3.16)

project(drag_drop_test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)
qt_policy(SET QTP0004 NEW)

qt_add_executable(appdrag_drop_test
    main.cpp
)

file(GLOB_RECURSE QML_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "*.qml"
    "*.js"
)

list(FILTER QML_FILES EXCLUDE REGEX "build/")

file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp"
    "src/*.h"
)

qt_add_qml_module(appdrag_drop_test
    URI drag_drop_test
    VERSION 1.0
    QML_FILES ${QML_FILES}
    SOURCES ${SOURCE_FILES}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.


target_link_libraries(appdrag_drop_test
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appdrag_drop_test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
