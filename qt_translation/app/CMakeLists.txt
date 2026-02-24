qt_add_executable(qt_translation_app
    MANUAL_FINALIZATION
    main.cpp
)

target_link_libraries(qt_translation_app
    PRIVATE
        Qt6::Quick
        translationlib
)

qt_import_qml_plugins(qt_translation_app)
qt_finalize_executable(qt_translation_app)
