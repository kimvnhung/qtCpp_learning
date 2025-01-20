#ifndef COMMON_H
#define COMMON_H

#include <QSettings>
#include <QStandardPaths>
#include <QMessageBox>

// Define a macro for a simple message box with a title, text, and "OK" button
#define SHOW_MESSAGE_BOX(TITLE, TEXT) \
{ \
        QMessageBox msgBox; \
        msgBox.setWindowTitle(TITLE); \
        msgBox.setModal(true); \
        msgBox.setText(TEXT); \
        msgBox.setStandardButtons(QMessageBox::Ok); \
        msgBox.exec(); \
}

#define APP_SETTINGS() QSettings("MySettings", "image_voter")

#define GETS(KEY, DFVALUE) APP_SETTINGS().value(KEY, DFVALUE)
#define SETS(KEY, VALUE) APP_SETTINGS().setValue(KEY, VALUE)

#define GET_STR(KEY) GETS(KEY, "").toString()
#define GET_INT(KEY) GETS(KEY, 0).toInt()
#define GET_DOUBLE(KEY) GETS(KEY, 0.0).toDouble()

#define LAST_PATH_KEY "last_path"
#define LAST_PATH_DEFAULT_VALUE ""

#define GET_LAST_PATH() GETS(LAST_PATH_KEY, LAST_PATH_DEFAULT_VALUE).toString()
#define SET_LAST_PATH(VALUE) SETS(LAST_PATH_KEY, VALUE)

#endif // COMMON_H
