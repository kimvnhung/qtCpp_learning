cmake_minimum_required(VERSION 3.25)
project(camera_demo LANGUAGES CXX)

add_executable(camera_demo
    src/main.cpp
)

target_include_directories(camera_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(camera_demo PRIVATE cxx_std_20)

target_link_libraries(camera_demo PRIVATE camera_core)

find_package(Qt6 COMPONENTS Quick REQUIRED)
target_link_libraries(camera_demo PRIVATE Qt6::Quick)

install(TARGETS camera_demo RUNTIME DESTINATION bin)
