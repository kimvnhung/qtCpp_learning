# cmake_minimum_required(VERSION 3.5)

# # Set the source and include directories
# set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# # Find header files in the include directory
# file(GLOB HEADER_FILES "${INCLUDE_DIR}/*.h")

# # Find source files in the source directory
# file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

# # Create the library target
# add_library(rtsp_module ${SOURCE_FILES} ${HEADER_FILES})

# # Include the include directory
# target_include_directories(rtsp_module PUBLIC ${INCLUDE_DIR})

# target_link_libraries(rtsp_module 
#                     PRIVATE
#                     Qt${QT_VERSION_MAJOR}::Core
# )

# Set any additional library-specific configurations
# For example, setting compiler options or linking against other libraries
# target_compile_options(MyLibrary PRIVATE -Wall)
# target_link_libraries(MyLibrary PRIVATE other_library)
