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

# copy to use folder
# set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../normal_library/${PROJECT_NAME})

# install(TARGETS ${PROJECT_NAME}
# LIBRARY DESTINATION ${INSTALL_DIR}/lib
# RUNTIME DESTINATION ${INSTALL_DIR}/bin
# INCLUDES DESTINATION ${INSTALL_DIR}/include
# )