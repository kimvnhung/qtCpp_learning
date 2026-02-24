qt_add_executable(qt_translation_app
    MANUAL_FINALIZATION
    main.cpp
)

message(STATUS "qm_files: ${qm_files}")
if(qm_files)
    set(QM_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/translations")
    file(MAKE_DIRECTORY ${QM_OUTPUT_DIR})
    add_custom_command(TARGET qt_translation_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${qm_files}
            ${QM_OUTPUT_DIR}
        COMMENT "Copying QM files to output directory"
    )
endif()

target_link_libraries(qt_translation_app
    PRIVATE
        Qt6::Quick
        translationlib
)

target_include_directories(qt_translation_app
    PRIVATE
        ${CMAKE_SOURCE_DIR}/lib
)

qt_import_qml_plugins(qt_translation_app)
qt_finalize_executable(qt_translation_app)
