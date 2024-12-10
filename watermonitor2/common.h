
#ifndef COMMON_H
#define COMMON_H

#include <QSettings>
#include <QWidget>

#define QSETIINGS QSettings("watersetting", "watersetting")
#define SET_VALUE(key, value) QSETIINGS.setValue(key, value)
#define GET_VALUE(key) QSETIINGS.value(key)

#define GET_STRING(key) GET_VALUE(key).toString()
#define GET_INT(key) GET_VALUE(key).toInt()
#define GET_BOOL(key) GET_VALUE(key).toBool()
#define GET_DOUBLE(key) GET_VALUE(key).toDouble()

#define LOGE(msg) qCritical() <<__FUNCTION__ << __LINE__ << msg
#define LOGD(msg) qDebug() << __FUNCTION__ << __LINE__ << msg
#define LOGI(msg) qInfo() << __FUNCTION__ << __LINE__ << msg
#define LOGW(msg) qWarning() << __FUNCTION__ << __LINE__ << msg

#define LOG() LOGD("")

// SETING KEYS
#define CSV_FILE_PATH "csv_file_path"
#define SETTING_COLLAPSED "setting_collapsed"


#define SHOW_PROGESS_VALUE 0
#define HIDE_PROGRESS_VALUE -1

#define ALL_TIME_TF_INDEX 0
#define LAST_WEEK_TF_INDEX 1
#define LAST_MONTH_TF_INDEX 2
#define LAST_3_MONTHS_TF_INDEX 3
#define LAST_6_MONTHS_TF_INDEX 4
#define LAST_YEAR_TF_INDEX 5




#endif // COMMON_H
