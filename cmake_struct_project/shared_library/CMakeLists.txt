cmake_minimum_required(VERSION 3.5)

# Get the name of the folder containing the CMakeLists.txt file
get_filename_component(DYNAMIC_PR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Consider changing the project name to something relevant for you.
project(${DYNAMIC_PR_NAME})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.h")

# Tell cmake we want it to automate generating an export stub for the dll
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

set(INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)

# Specify the directories to install headers
include(../cmake/functions.cmake)
configure_files(${SOURCE_DIR} "${INCLUDE_DIR}/${PROJECT_NAME}" ".h")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# Set the output directory for the shared library
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)

# Set the output directory for the library
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)



install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin
    LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib
)
