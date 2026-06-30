cmake_minimum_required(VERSION 3.25)
project(camera_renderer LANGUAGES CXX)

option(CAMERA_RENDERER_SHARED "Build renderer as shared library" ${CAMERA_BUILD_SHARED})

set(renderer_sources
    src/VideoItem.cpp
    src/VideoMaterial.cpp
)

# uploader implementations
list(APPEND renderer_sources
    src/uploaders/QSGUploader.cpp
)

if(CAMERA_RENDERER_SHARED)
    add_library(camera_renderer SHARED ${renderer_sources})
else()
    add_library(camera_renderer STATIC ${renderer_sources})
endif()

target_include_directories(camera_renderer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
)

target_compile_features(camera_renderer PUBLIC cxx_std_20)

# Find Qt6 Quick/Gui for QSGGeometryNode-based renderer
find_package(Qt6 COMPONENTS Quick Gui Core REQUIRED)

target_link_libraries(camera_renderer PUBLIC camera_core Qt6::Quick Qt6::Gui Qt6::Core)

set_target_properties(camera_renderer PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

install(TARGETS camera_renderer
        EXPORT cameraTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

