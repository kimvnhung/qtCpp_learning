cmake_minimum_required(VERSION 3.25)
project(camera_core LANGUAGES CXX)

option(CAMERA_CORE_SHARED "Build camera_core as shared library" ${CAMERA_BUILD_SHARED})

set(core_sources
    src/VideoFrame.cpp
    src/VideoBuffer.cpp
    src/MemoryPool.cpp
)

if(CAMERA_CORE_SHARED)
    add_library(camera_core SHARED ${core_sources})
else()
    add_library(camera_core STATIC ${core_sources})
endif()

target_include_directories(camera_core
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
)

target_compile_features(camera_core PUBLIC cxx_std_20)

# Install rules for headers and target
install(TARGETS camera_core
        EXPORT cameraTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)
