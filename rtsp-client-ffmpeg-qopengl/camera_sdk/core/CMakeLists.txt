cmake_minimum_required(VERSION 3.25)
project(camera_core LANGUAGES CXX)

set(core_sources
    src/VideoFrame.cpp
    src/VideoBuffer.cpp
    src/MemoryPool.cpp
    src/TripleBufferFrameQueue.cpp
)

add_library(camera_core SHARED ${core_sources})

target_include_directories(camera_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${FFMPEG_INCLUDES}
)
target_link_libraries(camera_core
    PUBLIC
        ${FFMPEG_LIBS}
)

# Install rules for headers and target
install(TARGETS camera_core
        EXPORT cameraTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)
