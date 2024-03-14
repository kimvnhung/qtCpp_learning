cmake_minimum_required(VERSION 3.5)
project(MyLibrary)

# Set the source and include directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find header files in the include directory
file(GLOB HEADER_FILES "${INCLUDE_DIR}/*.h")

# Find source files in the source directory
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

# Create the library target
add_library(MyLibrary ${SOURCE_FILES} ${HEADER_FILES})

# Copy header files to a specific directory
foreach(HEADER ${HEADER_FILES})
    configure_file(${HEADER} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

# Include the include directory
target_include_directories(MyLibrary
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/../
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/../
)
