cmake_minimum_required(VERSION 3.16)

project(hkv_reuse_streaming VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 5.15 REQUIRED COMPONENTS Quick Core Gui Widgets Qml QuickWidgets)


# Create the executable
set(
    SOURCES
        main.cpp
        mainwindow.cpp
        resourcecontainer.cpp
)

set(
    HEADERS
        mainwindow.h
        resourcecontainer.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}
    demo.h demo.cpp)


# Add the QML resources to the target
target_sources(${PROJECT_NAME} PRIVATE ${QML_RESOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Qml Qt5::Quick Qt5::QuickWidgets)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

