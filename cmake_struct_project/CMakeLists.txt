cmake_minimum_required(VERSION 3.5)
project(cmake_struct_project)

# Add the library project
add_subdirectory(shared_library)
add_subdirectory(static_library)

add_subdirectory(normal_library)


# Set the source directory to the root directory containing CMakeLists.txt
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Find source files in the source directory
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

# Create the main executable
add_executable(cmake_struct_project ${SOURCE_FILES})

# Include the include directory of the library

# Link against the library target
target_link_libraries(cmake_struct_project PRIVATE normal_library)
