cmake_minimum_required(VERSION 3.21)

project(qt_translation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick LinguistTools)
qt_standard_project_setup(REQUIRES 6.5
    I18N_TRANSLATED_LANGUAGES en vi
)

set(TS_DIR "${CMAKE_SOURCE_DIR}/translations")
set(TS_FILES
    "${TS_DIR}/app_en.ts"
    "${TS_DIR}/app_vi.ts"
)

add_subdirectory(lib)
add_subdirectory(app)

qt_add_translations(qt_translation_app
    TS_FILES ${TS_FILES}
    QM_FILES_OUTPUT_VARIABLE qm_files
    SOURCES
        ${CMAKE_SOURCE_DIR}/app/main.cpp
        ${CMAKE_SOURCE_DIR}/lib/backend.h
        ${CMAKE_SOURCE_DIR}/lib/backend.cpp
        ${CMAKE_SOURCE_DIR}/lib/languagemanager.h
        ${CMAKE_SOURCE_DIR}/lib/languagemanager.cpp
        ${CMAKE_SOURCE_DIR}/lib/qml/MainView.qml
)

# Build ID-based qm files for qtTrId()/qsTrId() without breaking normal tr()/qsTr().
set(id_qm_files "")
foreach(ts_file IN LISTS TS_FILES)
    get_filename_component(ts_base "${ts_file}" NAME_WLE)
    set(id_qm "${CMAKE_CURRENT_BINARY_DIR}/${ts_base}_id.qm")
    add_custom_command(
        OUTPUT "${id_qm}"
        COMMAND "$<TARGET_FILE:Qt6::lrelease>" -idbased "${ts_file}" -qm "${id_qm}"
        DEPENDS Qt6::lrelease "${ts_file}"
        VERBATIM
    )
    list(APPEND id_qm_files "${id_qm}")
endforeach()

add_custom_target(qt_translation_app_lrelease_idbased DEPENDS ${id_qm_files})
add_dependencies(qt_translation_app qt_translation_app_lrelease_idbased)

