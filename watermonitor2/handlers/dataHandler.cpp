#include "dataHandler.hpp"

#include "csv.hpp"
#include "filter.hpp"
#include "../common.h"

DataHandler::DataHandler(QObject *parent)
    : QThread(parent)
{
}

DataHandler::~DataHandler()
{
}

QList<Filter> DataHandler::filters() const
{
    return m_filters;
}

void DataHandler::run()
{
    LOG();
    m_isRunning = true;
    setIsFilteredChanged(true);
    while (m_isRunning)
    {
        // Do something
        if(m_filename.isEmpty())
        {
            continue;
        }



        if(!m_isLoaded)
        {
            m_isLoaded = loading();
        }

        if(m_isLoaded)
        {
            if(isFilteredChanged())
            {
                doFilter();
            }
        }
        QThread::msleep(500);
    }

}

void DataHandler::stop()
{
    LOG();
    m_isRunning = false;
}


void DataHandler::takePollutantOverviewData()
{
    LOG();

}

void DataHandler::takeComplianceDashboardData()
{

}

void DataHandler::takePOPData()
{


}

void DataHandler::takeFluorinatedCompoundsData()
{


}

void DataHandler::takeEnvironmentalLitterIndicatorsData()
{
    LOGD(QString("m_isFilteredChanged %1").arg(m_isFilteredChanged));
    QStringList locations;
    QStringList materials;

    emit handling(SHOW_PROGESS_VALUE);
    emit processingMessage("Processing chart data...");

    for(const auto &water : m_filteredData)
    {
        // Get unqiue locations and materials

        if(!locations.contains(QString::fromStdString(water.getLocation())))
        {
            locations.append(QString::fromStdString(water.getLocation()));
        }

        if(!materials.contains(QString::fromStdString(water.getDeterminand())))
        {
            materials.append(QString::fromStdString(water.getDeterminand()));
        }
    }

    emit handling(30);

    // Init map
    QMap<QString,QList<double>> avgResults; // <location, <material,results>>
    // Number of material sample at location
    QMap<QString,QList<int>> counts; // <location, <material,counts>>


    for (const auto &location : locations)
    {
        avgResults[location] = QList<double>(materials.size(), 0);
        counts[location] = QList<int>(materials.size(), 0);
    }

    emit handling(50);

    if(locations.isEmpty())
    {
        emit environmentalLitterIndicatorsDataReady(locations, materials, avgResults, 100);
        setIsFilteredChanged(false);
        emit handling(HIDE_PROGRESS_VALUE);
        return;
    }

    for(const auto &water : m_filteredData)
    {
        int materialIndex = materials.indexOf(QString::fromStdString(water.getDeterminand()));

        if(materialIndex == -1)
        {
            LOGD("Material not found");
            continue;
        }

        if(locations.contains(QString::fromStdString(water.getLocation()))){
            avgResults[QString::fromStdString(water.getLocation())][materialIndex] += water.getResult();
            counts[QString::fromStdString(water.getLocation())][materialIndex] += 1;
        }
    }

    emit handling(70);

    // Calculate average
    for (const auto &location : locations)
    {
        for (int i = 0; i < materials.size(); ++i)
        {
            if(counts[location][i] != 0)
            {
                avgResults[location][i] /= counts[location][i];
            }
            else
                avgResults[location][i] = 0;
        }
    }

    emit handling(90);

    // Find max value
    double maxValue = 0;
    for (const auto &location : locations)
    {
        double sum = 0;
        for (int i = 0; i < materials.size(); ++i)
        {
            sum += avgResults[location][i];
        }

        if(sum > maxValue)
        {
            maxValue = sum;
        }
    }
    emit handling(HIDE_PROGRESS_VALUE);
    emit processingMessage("");
    emit environmentalLitterIndicatorsDataReady(locations, materials, avgResults, maxValue);
    setIsFilteredChanged(false);
}

void DataHandler::setIsFilteredChanged(bool changed)
{
    // QMutexLocker locker(&mutex);
    LOGD(QString("changed %1").arg(changed));
    m_isFilteredChanged = changed;
}

bool DataHandler::isFilteredChanged()
{
    // QMutexLocker locker(&mutex);
    return m_isFilteredChanged;
}

void DataHandler::doFilter()
{
    LOGD(QString("m_filters.size() %1, m_rootData.size() %2").arg(m_filters.size()).arg(m_rootData.size()));
    m_filteredData.clear();
    if(m_rootData.empty())
    {
        setIsFilteredChanged(false);
        return;
    }

    if(m_filters.size() == 0)
    {
        m_filteredData = m_rootData;
        emit rawDataReady(m_filteredData);
        setIsFilteredChanged(false);
        return;
    }

    for(int i=0; i<m_filters.size();i++)
    {
        auto filter = m_filters[i];
        switch (filter.type())
        {
        case Filter::FilterType::LOCATIONS:
            break;
        case Filter::FilterType::MATERIALS:
            break;
        case Filter::FilterType::LOCATIONS_CONTAIN:
            break;
        case Filter::FilterType::MATERIALS_CONTAIN:
            break;
        case Filter::FilterType::LOCATIONS_SET:
            break;
        case Filter::FilterType::MATERIALS_SET:
            break;
        default:
            break;
        }
    }

    LOGD(QString("m_filteredData.size() %1").arg(m_filteredData.size()));
    emit rawDataReady(m_filteredData);

    takeEnvironmentalLitterIndicatorsData();
    setIsFilteredChanged(false);
}

bool DataHandler::loading()
{
    std::ifstream file(m_filename.toStdString());
    size_t rowCount = 0;
    std::string line;

    while (std::getline(file, line)) {
        ++rowCount;
    }

    if(rowCount == 0)
    {
        emit handling(HIDE_PROGRESS_VALUE);
        return false;
    }

    csv::CSVReader reader(m_filename.toStdString());
    m_rootData.clear();

    int count = 0;

    QDateTime compareDate = QDateTime::currentDateTime();

    QStringList locations;
    QStringList materials;

    for (const auto &row : reader)
    {
        Water water{
            row["@id"].get<std::string>(),
            row["sample.samplingPoint.label"].get<std::string>(),
            row["sample.sampleDateTime"].get<std::string>(),
            row["determinand.label"].get<std::string>(),
            row["result"].get<double>(),
            row["determinand.unit.label"].get<std::string>(),
            row["sample.sampledMaterialType.label"].get<std::string>(),
            QString::fromStdString(row["sample.isComplianceSample"].get<std::string>()).toLower() == "true"
        };

        m_rootData.push_back(water);
        if(!locations.contains(QString::fromStdString(water.getLocation())))
        {
            locations.append(QString::fromStdString(water.getLocation()));
        }

        if(!materials.contains(QString::fromStdString(water.getDeterminand())))
        {
            materials.append(QString::fromStdString(water.getDeterminand()));
        }

        emit handling((int)(count++*100/rowCount));
        if(!m_isRunning)
            return false;
    }

    emit locationsChanged(locations);
    emit materialsChanged(materials);

    emit handling(HIDE_PROGRESS_VALUE);
    emit dataReady();

    file.close();

    return !m_rootData.empty();
}

void DataHandler::loadData(QString filename)
{
    LOG();
    m_filename = filename;
    m_isLoaded = false;
}

void DataHandler::addFilter(const Filter& filter)
{
    LOG();
    m_filters.push_back(filter);
    setIsFilteredChanged(true);
}

void DataHandler::removeFilter(const Filter& filter)
{
    LOG();
    m_filters.removeAll(filter);
}

void DataHandler::takeGeographicalData()
{


}
