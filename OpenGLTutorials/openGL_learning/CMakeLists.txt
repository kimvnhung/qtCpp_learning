cmake_minimum_required(VERSION 3.5)
project(openGL_learning)

# Add the library project
add_subdirectory(my_library)

set(
    MY_LIBRARY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/my_library/include
)

# Set the source directory to the root directory containing CMakeLists.txt
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Find source files in the source directory
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

## glew
if(WIN32)
    set(GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glew)
    file(GLOB GLEW_LIBS ${GLEW_DIR}/lib/Release/Win32/*.lib)
    set(GLEW_INCLUDE ${GLEW_DIR}/include)
endif()

## ogldev
if(WIN32)
    set(OGLDEV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ogldev)
    set(OGLDEV_INCLUDE ${OGLDEV_DIR}/include)
endif()

# Create the main executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include the include directory of the library
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${MY_LIBRARY_INCLUDE}
        ${GLEW_INCLUDE}
        ${OGLDEV_INCLUDE}
)

# Link against the library target
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MyLibrary
        ${GLEW_LIBS}
)
