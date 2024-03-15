cmake_minimum_required(VERSION 3.5)

# Get the name of the folder containing the CMakeLists.txt file
get_filename_component(DYNAMIC_PR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Consider changing the project name to something relevant for you.
project(${DYNAMIC_PR_NAME})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${HEADERS})

# Set the output directory for the library
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# Specify the directories to install headers
include(../cmake/functions.cmake)
configure_files(${SOURCE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/include" ".h")


# Optionally, you can install the library
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)
