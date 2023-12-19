cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR) #< This version is bundled with MSVC 2019.


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(project_links VERSION 0.1)

add_subdirectory(hkv_reuse_streaming)
add_subdirectory(nxopen_env)
