cmake_minimum_required(VERSION 3.21)

project(qt_translation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick LinguistTools)
qt_standard_project_setup(REQUIRES 6.5
    I18N_TRANSLATED_LANGUAGES en vi
)

add_subdirectory(lib)
add_subdirectory(app)

set(TS_DIR "${CMAKE_SOURCE_DIR}/translations")
qt_add_translations(translationlib
    TS_FILE_DIR ${TS_DIR}
    TS_FILE_BASE app
    QM_FILES_OUTPUT_VARIABLE qm_files
)
