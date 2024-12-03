cmake_minimum_required(VERSION 3.16)

project(cwk2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)


find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network)
qt_standard_project_setup()

qt_add_executable(waterqualitymonitor
    main.cpp
    water.cpp
    dataset.cpp
    model.cpp
    window.cpp
    stats.cpp
    dashboardpage.cpp
    POPpage.cpp
    pollutantOverviewPage.cpp
    complianceDashboardPage.cpp
    fluorinatedCompoundsPage.cpp
    environmentalLitterIndicatorsPage.cpp
    geographicalHotspotsPage.cpp
    rawDataPage.cpp
    ui_elements.cpp
    interactive_card.cpp
)

target_link_libraries(waterqualitymonitor PRIVATE Qt6::Widgets PRIVATE Qt6::Core Qt6::Network)

set_target_properties(waterqualitymonitor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
