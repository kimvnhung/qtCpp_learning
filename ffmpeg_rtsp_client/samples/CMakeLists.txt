cmake_minimum_required(VERSION 3.18)
cmake_policy(VERSION 3.18)

## Get project name by folder name
get_filename_component(DYNAMIC_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "Project name: ${DYNAMIC_NAME}")
project(${DYNAMIC_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(testPTSAVFrame)
add_subdirectory(testDetectHardware)
