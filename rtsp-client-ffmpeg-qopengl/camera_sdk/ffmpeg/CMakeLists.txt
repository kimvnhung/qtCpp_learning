
cmake_minimum_required(VERSION 3.25)
project(camera_ffmpeg LANGUAGES CXX)

option(CAMERA_FFMPEG_SHARED "Build ffmpeg integration as shared library" ${CAMERA_BUILD_SHARED})

set(ffmpeg_sources
    src/FFmpegDecoder.cpp
)

if(CAMERA_FFMPEG_SHARED)
    add_library(camera_ffmpeg SHARED ${ffmpeg_sources})
else()
    add_library(camera_ffmpeg STATIC ${ffmpeg_sources})
endif()

target_include_directories(camera_ffmpeg
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
)

target_compile_features(camera_ffmpeg PUBLIC cxx_std_20)

# Find FFmpeg via pkg-config when available (portable across Linux/Android builds)
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(AVFORMAT libavformat QUIET)
    pkg_check_modules(AVCODEC libavcodec QUIET)
    pkg_check_modules(AVUTIL libavutil QUIET)
else()
    set(AVFORMAT_FOUND FALSE)
endif()

if(AVFORMAT_FOUND)
    target_include_directories(camera_ffmpeg PRIVATE ${AVFORMAT_INCLUDE_DIRS} ${AVCODEC_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS})
    target_link_libraries(camera_ffmpeg PRIVATE ${AVFORMAT_LIBRARIES} ${AVCODEC_LIBRARIES} ${AVUTIL_LIBRARIES})
endif()

target_link_libraries(camera_ffmpeg PUBLIC camera_core)

install(TARGETS camera_ffmpeg
        EXPORT cameraTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

