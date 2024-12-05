#pragma once

#ifndef COMMON_HPP
#define COMMON_HPP

#include <QSettings>
#include <QWidget>

#define QSETIINGS QSettings("watermonitor", "watermonitor")
#define SET_VALUE(key, value) QSETIINGS.setValue(key, value)
#define GET_VALUE(key) QSETIINGS.value(key)

#define GET_STRING(key) GET_VALUE(key).toString()
#define GET_INT(key) GET_VALUE(key).toInt()
#define GET_BOOL(key) GET_VALUE(key).toBool()
#define GET_DOUBLE(key) GET_VALUE(key).toDouble()

#define LOGE(msg) qCritical() << __FUNCTION__ << __LINE__ << msg
#define LOGD(msg) qDebug() << __FUNCTION__ << __LINE__ << msg
#define LOGI(msg) qInfo() << __FUNCTION__ << __LINE__ << msg
#define LOGW(msg) qWarning() << __FUNCTION__ << __LINE__ << msg

#define LOG() LOGD("")

// SETING KEYS
#define CSV_FILE_PATH "csv_file_path"

//
#define SHOW_PROGESS_VALUE 0
#define HIDE_PROGRESS_VALUE -1
#define MAX_PROGRESS_VALUE 100

void acceptClickEvents(QWidget *widget, bool isAccept = true);

#endif // COMMON_HPP
