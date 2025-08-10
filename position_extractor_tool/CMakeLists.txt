cmake_minimum_required(VERSION 3.16)

project(position_extractor_tool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)


qt_add_executable(appposition_extractor_tool
    main.cpp
)

qt_add_qml_module(appposition_extractor_tool
    URI position_extractor_tool
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        SOURCES appmodel.h appmodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appposition_extractor_tool PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appposition_extractor_tool
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appposition_extractor_tool
    PRIVATE Qt6::Quick
)
target_link_libraries(appposition_extractor_tool PRIVATE Qt6::Core)
target_link_libraries(appposition_extractor_tool PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appposition_extractor_tool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
