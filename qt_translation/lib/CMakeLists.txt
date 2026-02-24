# Set Qt QTP0001 policy to avoid warnings about using Qt modules without importing them in QML files
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_qml_module(translationlib
    URI TranslationLib
    VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/TranslationLib"

    SOURCES
        backend.h
        backend.cpp

    QML_FILES
        qml/MainView.qml
)

target_link_libraries(translationlib
    PRIVATE
        Qt6::Quick
)
