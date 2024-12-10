cmake_minimum_required(VERSION 3.16)

project(water VERSION 0.1 LANGUAGES CXX)

# Enable automatic Qt processing
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Charts)

# Translation files
set(TS_FILES
    water_en.ts
    water_es.ts
    water_fr.ts
    water_de.ts
)

file(GLOB CHART_SOURCES charts/*.cpp)
file(GLOB CHART_HEADERS charts/*.h)
file(GLOB HANDLERS_SOURCES handlers/*.cpp)
file(GLOB HANDLERS_HEADERS handlers/*.hpp)

# Source files
set(PROJECT_SOURCES
    main.cpp
    dashboard.cpp
    dashboard.h
    settingpanel.h
    settingpanel.cpp
    overviewchartwidget.h
    overviewchartwidget.cpp
    generaldashboard.h
    generaldashboard.cpp
    statusbar.h
    statusbar.cpp
    images.qrc
    common.h
    common.cpp
    expendablewidget.h
    expendablewidget.cpp
    filtercombobox.h
    filtercombobox.cpp
    ${HANDLERS_SOURCES}
    ${HANDLERS_HEADERS}
    ${CHART_SOURCES}
    ${CHART_HEADERS}
)

# Generate .qm files from .ts files
qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

# Create executable target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(water
        ${PROJECT_SOURCES}
    )
else()
    add_executable(water
        ${PROJECT_SOURCES}
    )
endif()

# Copy .qm files to the output directory
add_custom_command(TARGET water POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:water>/translations
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/translations
        $<TARGET_FILE_DIR:water>/translations
)

# Link Qt libraries
target_link_libraries(water PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)

# Installation configuration
include(GNUInstallDirs)
install(TARGETS water
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(water)
endif()
