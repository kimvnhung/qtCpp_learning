cmake_minimum_required(VERSION 3.25)
project(camera_sdk VERSION 0.1 LANGUAGES CXX)

# SDK-wide options
option(CAMERA_BUILD_SHARED "Build camera libraries as shared" OFF)
option(CAMERA_BUILD_TESTS "Build tests and demo" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allow users to inject Conan-generated cmake paths if available
if(EXISTS "${CMAKE_SOURCE_DIR}/conan_paths.cmake")
	include("${CMAKE_SOURCE_DIR}/conan_paths.cmake")
elseif(EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
	include("${CMAKE_BINARY_DIR}/conan_paths.cmake")
endif()

# Expose an install/export namespace for downstream usage
set(CAMERA_INSTALL_CMAKEDIR "lib/cmake/camera" CACHE PATH "Installation directory for CMake files")

add_subdirectory(core)
add_subdirectory(ffmpeg)
add_subdirectory(consumer)
if(CAMERA_BUILD_TESTS)
	add_subdirectory(demo)
endif()

# Generate a top-level export so consumers can `find_package(camera CONFIG)` after install.
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/cameraConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/CameraConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cameraConfig.cmake"
	INSTALL_DESTINATION ${CAMERA_INSTALL_CMAKEDIR}
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cameraConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/cameraConfigVersion.cmake"
		DESTINATION ${CAMERA_INSTALL_CMAKEDIR})

install(EXPORT cameraTargets
		FILE cameraTargets.cmake
		NAMESPACE camera::
		DESTINATION ${CAMERA_INSTALL_CMAKEDIR})
