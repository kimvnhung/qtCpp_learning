cmake_minimum_required(VERSION 3.5)

project(rtsp_module)

#find Qt package

# Set the source and include directories
# set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find header files in the include directory
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Find source files in the source directory
file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Create the library target
add_library(rtsp_module SHARED ${SOURCE_FILES} ${HEADER_FILES})

# Include the include directory
target_include_directories(rtsp_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rtsp_module
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
    )
