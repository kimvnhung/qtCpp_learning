#pragma once

#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

#include <QObject>
#include <QThread>
#include <vector>
#include "water.hpp"
#include "dataset.hpp"

class DataHandler : public QThread
{
    Q_OBJECT
public:
    DataHandler(QObject *parent = nullptr);
    ~DataHandler();

    void loadData(const std::string &filename);

    WaterDataset getDataset() const;
    std::vector<Water> getData() const { return m_data; }

signals:
    void dataReady();
    void handling(int percent);
    void handlingGeographicalData(int percent);
    void geographicalDataReady(QStringList locations, QMap<QString,QList<int>> frequency, int min, int max);
public slots:
    void stop();

    void triggerGeographicalHotspots();
protected:
    void run() override;

private:
    std::vector<Water> m_data;
    QString m_filename;
    bool m_isRunning;

    bool m_isLoaded;
    bool m_isGeographicalHotspotsTriggered;

    bool loading();

    void takeGeographicalData();

};

#endif // DATAHANDLER_HPP
