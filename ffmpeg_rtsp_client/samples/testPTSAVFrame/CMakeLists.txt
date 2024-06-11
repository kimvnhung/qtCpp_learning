cmake_minimum_required(VERSION 3.18)

## Define folder name as project name
get_filename_component(DYNAMIC_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "Project name: ${DYNAMIC_NAME}")
project(${DYNAMIC_NAME})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${SOURCE_DIR}/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBS})
