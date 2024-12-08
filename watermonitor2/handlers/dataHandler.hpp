#pragma once

#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

#include <QObject>
#include <QThread>
#include <vector>
#include "water.hpp"

class DataHandler : public QThread
{
    Q_OBJECT
public:
    DataHandler(QObject *parent = nullptr);
    ~DataHandler();

    std::vector<Water> getData() const { return m_data; }

signals:
    void dataReady();
    void chartDataReady();
    void handling(int percent);
    void processingMessage(QString message);
    void environmentalLitterIndicatorsDataReady(QStringList locations, QStringList materials, QMap<QString,QList<double>> results, double maxValue);

    void locationsChanged(QStringList locations);
    void materialsChanged(QStringList materials);
public slots:
    void stop();

    void triggerGeographicalHotspots();
    void triggerPollutantOverview();
    void triggerComplianceDashboard();
    void triggerPOP();
    void triggerFluorinatedCompounds();
    void triggerEnvironmentalLitterIndicators();

    void loadData(QString filename);
    void setFilteredLocations(QStringList locations);
    void setFilteredMaterials(QStringList materials);

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

    bool m_isFilteredChanged;

    bool loading();

    void takeGeographicalData();
    void takePollutantOverviewData();
    void takeComplianceDashboardData();
    void takePOPData();
    void takeFluorinatedCompoundsData();
    void takeEnvironmentalLitterIndicatorsData();

    QStringList m_filteredLocations;
    QStringList m_filteredMaterials;
};

#endif // DATAHANDLER_HPP
