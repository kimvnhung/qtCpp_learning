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

    void handlingPollutantOverviewData(int percent);
    void pollutantOverviewDataReady(QStringList materials, QList<int> counts, QList<double> avgs, int maxCount, double maxAvg);

    void handlingComplianceDashboardData(int percent);
    void complianceDashboardDataReady(int trueCount, int falseCount);

    void handlingPOPData(int percent);
    void POPDataReady(QMap<QString, int> frequency);

    void handlingFluorinatedCompoundsData(int percent);
    void fluorinatedCompoundsDataReady(QMap<QString, int> frequency);

    void handlingEnvironmentalLitterIndicatorsData(int percent);
    void environmentalLitterIndicatorsDataReady(QMap<QString, int> frequency);

public slots:
    void stop();

    void triggerGeographicalHotspots();
    void triggerPollutantOverview();
    void triggerComplianceDashboard();
    void triggerPOP();
    void triggerFluorinatedCompounds();
    void triggerEnvironmentalLitterIndicators();

protected:
    void run() override;

private:
    std::vector<Water> m_data;
    QString m_filename;
    bool m_isRunning;

    bool m_isLoaded;
    bool m_isGeographicalHotspotsTriggered;
    bool m_isPollutantOverviewTriggered;
    bool m_isComplianceDashboardTriggered;
    bool m_isPOPTriggered;
    bool m_isFluorinatedCompoundsTriggered;
    bool m_isEnvironmentalLitterIndicatorsTriggered;

    bool loading();

    void takeGeographicalData();
    void takePollutantOverviewData();
    void takeComplianceDashboardData();
    void takePOPData();
    void takeFluorinatedCompoundsData();
    void takeEnvironmentalLitterIndicatorsData();

};

#endif // DATAHANDLER_HPP
