cmake_minimum_required(VERSION 3.5)

# Get the name of the folder containing the CMakeLists.txt file
get_filename_component(DYNAMIC_PR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# Consider changing the project name to something relevant for you.
project(${DYNAMIC_PR_NAME})

# Set the source and include directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find header files in the include directory
file(GLOB HEADER_FILES "${SOURCE_DIR}/*.h")

# Find source files in the source directory
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

add_subdirectory(sub_normal_library)

include_directories()

# Create the library target
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})


# ============================= Add static library ==========================
set(STATIC_LIB ${CMAKE_CURRENT_BINARY_DIR}/../static_library)

# ===================================  end  ====================================

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${STATIC_LIB}
)

# Copy header files to a specific directory
foreach(HEADER ${HEADER_FILES})
    configure_file(${HEADER} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

# Include the include directory
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/../
)
