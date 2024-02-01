cmake_minimum_required(VERSION 3.19)
project(stream_tools)
set(CMAKE_CXX_STANDARD 17)

find_library(AVCODEC_LIBRARY avcodec)

add_library(${PROJECT_NAME} SHARED src1, src2…)
target_link_libraries(${PROJECT_NAME} ${AVCODEC_LIBRARY})

add_executable(camera_encoder main.cc)
target_link_libraries(camera_encoder ${PROJECT_NAME})