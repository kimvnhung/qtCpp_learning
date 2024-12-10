#pragma once

#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

#include <QMutex>
#include <QObject>
#include <QThread>
#include <vector>
#include "water.hpp"

struct Filter;
enum class FilterType;

class DataHandler : public QThread
{
    Q_OBJECT
public:
    DataHandler(QObject *parent = nullptr);
    ~DataHandler();

    QList<Filter> filters() const;

signals:
    void dataReady();
    void handling(int percent);
    void processingMessage(QString message);

    void locationsChanged(QStringList locations);
    void materialsChanged(QStringList materials);

    void environmentalLitterIndicatorsDataReady(QStringList locations, QStringList materials, QMap<QString,QList<double>> results, double maxValue);
    void rawDataReady(std::vector<Water> data);

public slots:
    void stop();

    void loadData(QString filename);

    void addFilter(const Filter& filter);
    void removeFilter(const Filter& filter);

protected:
    void run() override;

private:
    std::vector<Water> m_rootData;
    std::vector<Water> m_filteredData;
    QString m_filename;
    bool m_isRunning;
    bool m_isLoaded;

    bool m_isFilteredChanged;
    void setIsFilteredChanged(bool changed);
    bool isFilteredChanged();
    QList<Filter> m_filters;
    void doFilter();

    bool loading();

    void takeGeographicalData();
    void takePollutantOverviewData();
    void takeComplianceDashboardData();
    void takePOPData();
    void takeFluorinatedCompoundsData();
    void takeEnvironmentalLitterIndicatorsData();
};

#endif // DATAHANDLER_HPP
