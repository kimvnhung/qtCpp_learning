#include "dataHandler.hpp"

#include "csv.hpp"
#include "../common.h"

DataHandler::DataHandler(QObject *parent)
    : QThread(parent)
{
}

DataHandler::~DataHandler()
{
}

void DataHandler::run()
{
    LOG();
    m_isRunning = true;
    bool markedForReloadChart = false;
    setIsFilteredChanged(false);
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
            markedForReloadChart = true;
        }

        if(m_isLoaded)
        {
            if(markedForReloadChart || isFilteredChanged())
            {
                // for waiting change in filter in fast way
                QThread::msleep(500);
                // takeGeographicalData();
                // takePollutantOverviewData();
                // takeComplianceDashboardData();
                // takePOPData();
                // takeFluorinatedCompoundsData();
                takeEnvironmentalLitterIndicatorsData();
                markedForReloadChart = false;
            }
        }
        QThread::msleep(100);
    }
    stop();
}

void DataHandler::stop()
{
    LOG();
    m_isRunning = false;
}

void DataHandler::triggerGeographicalHotspots()
{
    LOG();
    m_isGeographicalHotspotsTriggered = true;
}

void DataHandler::triggerPollutantOverview()
{
    LOG();
    m_isPollutantOverviewTriggered = true;
}

void DataHandler::triggerComplianceDashboard()
{
    LOG();
    m_isComplianceDashboardTriggered = true;
}

void DataHandler::triggerPOP()
{
    LOG();
    m_isPOPTriggered = true;
}

void DataHandler::triggerFluorinatedCompounds()
{
    LOG();
    m_isFluorinatedCompoundsTriggered = true;
}

void DataHandler::triggerEnvironmentalLitterIndicators()
{
    LOG();
    m_isEnvironmentalLitterIndicatorsTriggered = true;
}

void DataHandler::takePollutantOverviewData()
{
    LOG();
    QStringList materials;
    QList<int> counts;
    QList<double> avgs;
    int maxCount = 0;
    double maxAvg = 0;

    for (const auto &water : m_data)
    {
        if(!materials.contains(QString::fromStdString(water.getWaterType())))
        {
            materials.append(QString::fromStdString(water.getWaterType()));
        }
    }

    for (const auto &material : materials)
    {
        int count = 0;
        double sum = 0;
        for (const auto &water : m_data)
        {
            if (material == QString::fromStdString(water.getWaterType()))
            {
                count += 1;
                sum += water.getResult();
            }
        }
        counts.append(count);
        avgs.append(sum / count);
    }
}

void DataHandler::takeComplianceDashboardData()
{
    int trueCount = 0;
    int falseCount = 0;


    for (const auto &water : m_data)
    {
        if(water.isComplianceSample())
            trueCount++;
        else
            falseCount++;
    }
}

void DataHandler::takePOPData()
{
    QList<double> values = QList<double>(12,0);
    QList<double> counts = QList<double>(12,0);
    double max = 0;
    double min = 0;

    for (const auto &water : m_data)
    {
        // filter Persistent Organic Pollutants by name
        if(QString::fromStdString(water.getDeterminand()).contains("on"))
        {
            int month = water.getDateTime().date().month();
            values[month] += water.getResult();
            counts[month] += 1;
        }
    }

    for (int i = 0; i < values.size(); ++i)
    {
        values[i] = counts[i] ? values[i] / counts[i] : 0;
        if(values[i] > max)
        {
            max = values[i];
        }
        if(values[i] < min)
        {
            min = values[i];
        }
    }

}

void DataHandler::takeFluorinatedCompoundsData()
{
    QMap<QString, int> frequency; // <location, frequency>


    for (const auto &water : m_data)
    {
        frequency[QString::fromStdString(water.getLocation())] += 1;
    }

}

void DataHandler::takeEnvironmentalLitterIndicatorsData()
{
    LOGD(QString("m_isFilteredChanged %1").arg(m_isFilteredChanged));
    QStringList locations;
    QStringList materials;

    emit handling(SHOW_PROGESS_VALUE);
    emit processingMessage("Processing chart data...");

    if(isFilteredChanged())
    {
        locations = m_filteredLocations;
        materials = m_filteredMaterials;
    }
    else
    {
        for(const auto &water : m_data)
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

        emit locationsChanged(locations);
        emit materialsChanged(materials);

        m_filteredLocations = locations;
        m_filteredMaterials = materials;
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


    for(const auto &water : m_data)
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
    m_data.clear();

    int count = 0;

    QDateTime compareDate = QDateTime::currentDateTime();


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

        m_data.push_back(water);
        emit handling((int)(count++*100/rowCount));
        if(!m_isRunning)
            return false;
    }

    emit handling(HIDE_PROGRESS_VALUE);
    emit dataReady();

    file.close();

    return !m_data.empty();
}

void DataHandler::loadData(QString filename)
{
    LOG();
    m_filename = filename;
    // m_filterType = filterType;
    m_isLoaded = false;
}

void DataHandler::setFilteredLocations(QStringList locations)
{
    LOGD(QString("locations.count %1").arg(locations.count()));
    if(m_filteredLocations.count() != locations.count())
    {
        m_filteredLocations.clear();
        m_filteredLocations.append(locations);
        setIsFilteredChanged(true);
    }
}

void DataHandler::setFilteredMaterials(QStringList materials)
{
    LOGD(QString("m_filteredMaterials.count %1, materials.count %2").arg(m_filteredMaterials.count()).arg(materials.count()));
    if(m_filteredMaterials.count() != materials.count())
    {
        m_filteredMaterials.clear();
        m_filteredMaterials.append(materials);
        setIsFilteredChanged(true);
    }
}

void DataHandler::takeGeographicalData()
{
    QStringList locations;
    QMap<QString,QList<int>> frequency; // <month,frequency>



    int min = INT_MAX;
    int max = INT_MIN;

    // take unique locations
    for (const auto &water : m_data)
    {
        if(!locations.contains(QString::fromStdString(water.getLocation())))
        {
            locations.append(QString::fromStdString(water.getLocation()));
        }
    }

    // fill for all 12 months
    frequency["Jan"] = QList<int>(locations.size(), 0);
    frequency["Feb"] = QList<int>(locations.size(), 0);
    frequency["Mar"] = QList<int>(locations.size(), 0);
    frequency["Apr"] = QList<int>(locations.size(), 0);
    frequency["May"] = QList<int>(locations.size(), 0);
    frequency["Jun"] = QList<int>(locations.size(), 0);
    frequency["Jul"] = QList<int>(locations.size(), 0);
    frequency["Aug"] = QList<int>(locations.size(), 0);
    frequency["Sep"] = QList<int>(locations.size(), 0);
    frequency["Oct"] = QList<int>(locations.size(), 0);
    frequency["Nov"] = QList<int>(locations.size(), 0);
    frequency["Dec"] = QList<int>(locations.size(), 0);

    // take frequency of each location in each month
    for (const auto &water : m_data)
    {
        // Get month string like Jan, Feb, Mar, ...
        QString month = water.getDateTime().toString("MMM");

        int index = locations.indexOf(QString::fromStdString(water.getLocation()));
        LOGD(QString("month: %1, index: %2, frequency[%3].size %4").arg(month).arg(index).arg(month).arg(frequency[month].size()));
        frequency[month][index] += 1;
    }

    // find min and max frequency
    for (const auto &month : frequency)
    {
        for (const auto &freq : month)
        {
            if(freq < min)
            {
                min = freq;
            }
            if(freq > max)
            {
                max = freq;
            }
        }
    }

}
