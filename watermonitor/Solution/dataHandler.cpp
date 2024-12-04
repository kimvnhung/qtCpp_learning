#include "dataHandler.hpp"

#include "csv.hpp"
#include "common.hpp"

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
    while (m_isRunning)
    {
        // Do something
        if(m_filename.isEmpty())
        {
            continue;
        }

        if(!m_isLoaded)
            m_isLoaded = loading();

        if(m_isLoaded)
        {
            if(m_isGeographicalHotspotsTriggered)
            {
                takeGeographicalData();
                m_isGeographicalHotspotsTriggered = false;
            }

            if(m_isPollutantOverviewTriggered)
            {
                takePollutantOverviewData();
                m_isPollutantOverviewTriggered = false;
            }

            if(m_isComplianceDashboardTriggered)
            {
                takeComplianceDashboardData();
                m_isComplianceDashboardTriggered = false;
            }

            if(m_isPOPTriggered)
            {
                takePOPData();
                m_isPOPTriggered = false;
            }

            if(m_isFluorinatedCompoundsTriggered)
            {
                takeFluorinatedCompoundsData();
                m_isFluorinatedCompoundsTriggered = false;
            }

            if(m_isEnvironmentalLitterIndicatorsTriggered)
            {
                takeEnvironmentalLitterIndicatorsData();
                m_isEnvironmentalLitterIndicatorsTriggered = false;
            }
        }


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
    QStringList materials;
    QList<int> counts;
    QList<double> avgs;
    int maxCount = 0;
    double maxAvg = 0;

    emit handlingPollutantOverviewData(SHOW_PROGESS_VALUE);

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

    emit handlingPollutantOverviewData(99);
    emit pollutantOverviewDataReady(materials, counts, avgs, maxCount, maxAvg);
}

void DataHandler::takeComplianceDashboardData()
{
    int trueCount = 0;
    int falseCount = 0;

    emit handlingComplianceDashboardData(SHOW_PROGESS_VALUE);

    for (const auto &water : m_data)
    {
        if(water.isComplianceSample())
            trueCount++;
        else
            falseCount++;
    }

    emit handlingComplianceDashboardData(99);
    emit complianceDashboardDataReady(trueCount,falseCount);
}

void DataHandler::takePOPData()
{
    QMap<QString, int> frequency; // <month, frequency>

    emit handlingPOPData(SHOW_PROGESS_VALUE);

    for (const auto &water : m_data)
    {
        QString month = water.getDateTime().toString("MMM");
        frequency[month] += 1;
    }

    emit handlingPOPData(99);
    emit POPDataReady(frequency);
}

void DataHandler::takeFluorinatedCompoundsData()
{
    QMap<QString, int> frequency; // <location, frequency>

    emit handlingFluorinatedCompoundsData(SHOW_PROGESS_VALUE);

    for (const auto &water : m_data)
    {
        frequency[QString::fromStdString(water.getLocation())] += 1;
    }

    emit handlingFluorinatedCompoundsData(99);
    emit fluorinatedCompoundsDataReady(frequency);
}

void DataHandler::takeEnvironmentalLitterIndicatorsData()
{
    QMap<QString, int> frequency; // <location, frequency>

    emit handlingEnvironmentalLitterIndicatorsData(SHOW_PROGESS_VALUE);

    for (const auto &water : m_data)
    {
        frequency[QString::fromStdString(water.getLocation())] += 1;
    }

    emit handlingEnvironmentalLitterIndicatorsData(99);
    emit environmentalLitterIndicatorsDataReady(frequency);
}

WaterDataset DataHandler::getDataset() const
{
    return m_data;
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

    return !m_data.empty();
}

void DataHandler::loadData(const std::string &filename)
{
    LOG();
    m_filename = QString::fromStdString(filename);
    m_isLoaded = false;
}

void DataHandler::takeGeographicalData()
{
    QStringList locations;
    QMap<QString,QList<int>> frequency; // <month,frequency>


    emit handlingGeographicalData(SHOW_PROGESS_VALUE);

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

    // assume it took 30%
    emit handlingGeographicalData(30);

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

    // assume it took 70%
    emit handlingGeographicalData(70);
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

    // assume it took 100%
    emit handlingGeographicalData(99);
    emit geographicalDataReady(locations, frequency, min, max);
}
