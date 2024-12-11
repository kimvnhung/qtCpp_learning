#include "dataHandler.hpp"
#include "csv.hpp"
#include "filter.hpp"
#include "../common.h"
DataHandler::DataHandler(QObject *parent) : QThread(parent) {}
DataHandler::~DataHandler() {}
QList<Filter> DataHandler::filters() const { return m_filters; }
void DataHandler::run()
{
    LOG();
    m_isRunning = true;
    setIsFilteredChanged(true);
    while (m_isRunning)
    {
        if (m_filename.isEmpty())
        {
            continue;
        }
        if (!m_isLoaded)
        {
            // For refreshing UI
            setIsFilteredChanged(true);
            m_isLoaded = loading();

        }
        if (m_isLoaded)
        {
            if (isFilteredChanged())
            {
                QThread::msleep(500);
                doFilter();
            }
        }
        QThread::msleep(100);
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
    // emit handling(SHOW_PROGESS_VALUE);
    QStringList pollutants;
    for (const auto &water : m_filteredData)
    {
        if (!pollutants.contains(QString::fromStdString(water.getDeterminand())))
        {
            pollutants.append(QString::fromStdString(water.getDeterminand()));
        }
    }
    QList<QList<double>> valuesByMonth = QList<QList<double>>(12, QList<double>(pollutants.size(), 0));
    // emit handling(30);
    for (const auto &water : m_filteredData)
    {
        int month = water.getDateTime().date().month();
        int pollutantIndex = pollutants.indexOf(QString::fromStdString(water.getDeterminand()));
        if (pollutantIndex == -1)
        {
            LOGD("Pollutant not found");
            continue;
        }
        valuesByMonth[month - 1][pollutantIndex] += water.getResult();
    }
    // emit handling(70);
    double maxValue = 0;
    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < pollutants.size(); ++j)
        {
            if (valuesByMonth[i][j] > maxValue)
            {
                maxValue = valuesByMonth[i][j];
            }
        }
    }
    // emit handling(HIDE_PROGRESS_VALUE);
    emit pollutantOverviewDataReady(pollutants, valuesByMonth, maxValue);
}

void DataHandler::takeComplianceDashboardData()
{
    LOG();
    // emit handling(SHOW_PROGESS_VALUE);

    int trueCount = 0;
    int falseCount = 0;

    int count = 0;

    for (auto &water : m_filteredData)
    {
        if (water.isComplianceSample())
        {
            trueCount++;
        }
        else
        {
            falseCount++;
        }

        // emit handling(count++ * 100 / m_filteredData.size());
    }

    // emit handling(HIDE_PROGRESS_VALUE);
    emit complianceChartDataReady(trueCount, falseCount);

}

void DataHandler::takePOPsData()
{
    LOG();
    // emit handling(SHOW_PROGESS_VALUE);
    QStringList locations;
    for (const auto &water : m_filteredData)
    {
        if (!locations.contains(QString::fromStdString(water.getLocation())))
        {
            locations.append(QString::fromStdString(water.getLocation()));
        }
    }
    QList<QList<double>> valuesByMonth = QList<QList<double>>(12, QList<double>(locations.size(), 0));
    // emit handling(30);
    for (const auto &water : m_filteredData)
    {
        int month = water.getDateTime().date().month();
        int locationIndex = locations.indexOf(QString::fromStdString(water.getLocation()));
        if (locationIndex == -1)
        {
            LOGD("Location not found");
            continue;
        }
        valuesByMonth[month - 1][locationIndex] += water.getResult();
    }
    // emit handling(70);
    double maxValue = 0;
    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < locations.size(); ++j)
        {
            if (valuesByMonth[i][j] > maxValue)
            {
                maxValue = valuesByMonth[i][j];
            }
        }
    }
    // emit handling(HIDE_PROGRESS_VALUE);
    emit popsDataReady(locations, valuesByMonth, maxValue);
}


void DataHandler::takeFluorinatedCompoundsData()
{
    LOG();
    // emit handling(SHOW_PROGESS_VALUE);
    QStringList locations;
    for (const auto &water : m_filteredData)
    {
        if (!locations.contains(QString::fromStdString(water.getLocation())))
        {
            locations.append(QString::fromStdString(water.getLocation()));
        }
    }

    // emit handling(30);

    QList<double> lats = QList<double>(locations.size(), 0);
    QList<double> lons = QList<double>(locations.size(), 0);
    QList<double> values = QList<double>(locations.size(), 0);
    for (const auto &water : m_filteredData)
    {
        int locationIndex = locations.indexOf(QString::fromStdString(water.getLocation()));
        if (locationIndex == -1)
        {
            LOGD("Location not found");
            continue;
        }
        if (lats[locationIndex] == 0)
            lats[locationIndex] = water.getLat();
        if (lons[locationIndex] == 0)
            lons[locationIndex] = water.getLong();
        values[locationIndex] += water.getResult();
    }

    // emit handling(60);

    double minLat = 0, minLon = 0, maxLat = 0, maxLon = 0, maxValue = 0;

    for (int i = 0; i < locations.size(); i++)
    {
        if (i == 0)
        {
            minLat = lats[i];
            minLon = lons[i];
        }
        if (lats[i] < minLat)
            minLat = lats[i];
        if (lats[i] > maxLat)
            maxLat = lats[i];
        if (lons[i] < minLon)
            minLon = lons[i];
        if (lons[i] > maxLon)
            maxLon = lons[i];
        if (values[i] > maxValue)
            maxValue = values[i];
    }
    // emit handling(90);

    // emit handling(HIDE_PROGRESS_VALUE);
    emit flourinatedCompoundsChartDataReady(locations, lats, lons, values, minLat, minLon, maxLat, maxLon, maxValue);
}
void DataHandler::takeEnvironmentalLitterIndicatorsData()
{
    LOGD(QString("m_isFilteredChanged %1").arg(m_isFilteredChanged));
    QStringList locations;
    QStringList materials;
    // emit handling(SHOW_PROGESS_VALUE);
    // emit processingMessage("Processing chart data...");
    for (const auto &water : m_filteredData)
    {
        if (!locations.contains(QString::fromStdString(water.getLocation())))
        {
            locations.append(QString::fromStdString(water.getLocation()));
        }
        if (!materials.contains(QString::fromStdString(water.getDeterminand())))
        {
            materials.append(QString::fromStdString(water.getDeterminand()));
        }
    }
    // emit handling(30);
    QMap<QString, QList<double>> avgResults;
    QMap<QString, QList<int>> counts;
    for (const auto &location : locations)
    {
        avgResults[location] = QList<double>(materials.size(), 0);
        counts[location] = QList<int>(materials.size(), 0);
    }
    // emit handling(50);
    LOGD(QString("locations.size() %1, materials.size() %2").arg(locations.size()).arg(materials.size()));
    if (locations.isEmpty())
    {
        emit environmentalLitterIndicatorsDataReady(locations, materials, avgResults, 100);
        setIsFilteredChanged(false);
        // emit handling(HIDE_PROGRESS_VALUE);
        return;
    }
    for (const auto &water : m_filteredData)
    {
        int materialIndex = materials.indexOf(QString::fromStdString(water.getDeterminand()));
        if (materialIndex == -1)
        {
            LOGD("Material not found");
            continue;
        }
        if (locations.contains(QString::fromStdString(water.getLocation())))
        {
            avgResults[QString::fromStdString(water.getLocation())][materialIndex] += water.getResult();
            counts[QString::fromStdString(water.getLocation())][materialIndex] += 1;
        }
    }
    // emit handling(70);
    for (const auto &location : locations)
    {
        for (int i = 0; i < materials.size(); ++i)
        {
            if (counts[location][i] != 0)
            {
                avgResults[location][i] /= counts[location][i];
            }
            else
                avgResults[location][i] = 0;
        }
    }
    // emit handling(90);
    double maxValue = 0;
    for (const auto &location : locations)
    {
        double sum = 0;
        for (int i = 0; i < materials.size(); ++i)
        {
            sum += avgResults[location][i];
        }
        if (sum > maxValue)
        {
            maxValue = sum;
        }
    }
    // emit handling(HIDE_PROGRESS_VALUE);
    // emit processingMessage("");
    emit environmentalLitterIndicatorsDataReady(locations, materials, avgResults, maxValue);

}
void DataHandler::setIsFilteredChanged(bool changed)
{
    LOGD(QString("changed %1").arg(changed));
    m_isFilteredChanged = changed;
}
bool DataHandler::isFilteredChanged()
{
    return m_isFilteredChanged;
}
void DataHandler::doFilter()
{
    LOGD(QString("m_filters.size() %1, m_rootData.size() %2").arg(m_filters.size()).arg(m_rootData.size()));
    m_filteredData.clear();
    if (m_rootData.empty())
    {
        setIsFilteredChanged(false);
        return;
    }
    if (m_filters.size() == 0)
    {
        m_filteredData = m_rootData;
    }
    else
    {
        m_filteredData = m_rootData;
        std::vector<Water> tempFilter;
        for (int i = 0; i < m_filters.size(); i++)
        {
            auto filter = m_filters[i];
            switch (filter.type())
            {
            case Filter::FilterType::LOCATIONS:
            {
                QString location = filter.value().toString();
                for (const auto &water : m_filteredData)
                {
                    if (QString::fromStdString(water.getLocation()) == location)
                    {
                        tempFilter.push_back(water);
                    }
                }
                break;
            }
            case Filter::FilterType::MATERIALS:
            {
                QString material = filter.value().toString();
                for (const auto &water : m_filteredData)
                {
                    if (QString::fromStdString(water.getDeterminand()) == material)
                    {
                        tempFilter.push_back(water);
                    }
                }
                break;
            }
            case Filter::FilterType::LOCATIONS_CONTAIN:
            {
                QString location = filter.value().toString();
                for (const auto &water : m_filteredData)
                {
                    if (QString::fromStdString(water.getLocation()).contains(location))
                    {
                        tempFilter.push_back(water);
                    }
                }
                break;
            }
            case Filter::FilterType::MATERIALS_CONTAIN:
            {
                QString material = filter.value().toString();
                for (const auto &water : m_filteredData)
                {
                    if (QString::fromStdString(water.getDeterminand()).contains(material))
                    {
                        tempFilter.push_back(water);
                    }
                }
                break;
            }
            case Filter::FilterType::LOCATIONS_SET:
            {
                QStringList locations = filter.value().toStringList();
                for (const auto &water : m_filteredData)
                {
                    if (locations.contains(QString::fromStdString(water.getLocation())))
                    {
                        tempFilter.push_back(water);
                    }
                }
                break;
            }
            case Filter::FilterType::MATERIALS_SET:
            {
                QStringList materials = filter.value().toStringList();
                for (const auto &water : m_filteredData)
                {
                    if (materials.contains(QString::fromStdString(water.getDeterminand())))
                    {
                        tempFilter.push_back(water);
                    }
                }
                break;
            }
            case Filter::FilterType::TIME:
            {
                LOGD(QString("Before filter time: m_filteredData.size() %1").arg(m_filteredData.size()));
                int time = filter.value().toInt();
                QDateTime compareDate = QDateTime::currentDateTime();
                switch (time)
                {
                case LAST_WEEK_TF_INDEX:
                {
                    compareDate = QDateTime::currentDateTime().addDays(-7);
                    break;
                }
                case LAST_MONTH_TF_INDEX:
                {
                    compareDate = QDateTime::currentDateTime().addMonths(-1);
                    break;
                }
                case LAST_3_MONTHS_TF_INDEX:
                {
                    compareDate = QDateTime::currentDateTime().addMonths(-3);
                    break;
                }
                case LAST_6_MONTHS_TF_INDEX:
                {
                    compareDate = QDateTime::currentDateTime().addMonths(-6);
                    break;
                }
                case LAST_YEAR_TF_INDEX:
                {
                    compareDate = QDateTime::currentDateTime().addYears(-1);
                    break;
                }
                default:
                    break;
                }
                for (const auto &water : m_filteredData)
                {
                    if (time != ALL_TIME_TF_INDEX && water.getDateTime() >= compareDate)
                    {
                        tempFilter.push_back(water);
                    }
                }
                break;
            }
            case Filter::FilterType::LOCATIONS_MAX:
            {
                int maxLocations = filter.value().toInt();
                if(maxLocations == 0)
                {
                    break;
                }
                QStringList locations;
                for (const auto &water : m_filteredData)
                {
                    if(!locations.contains(QString::fromStdString(water.getLocation())))
                    {
                        locations.append(QString::fromStdString(water.getLocation()));
                    }

                    if(locations.size() >= maxLocations)
                    {
                        break;
                    }
                }

                for (const auto &water : m_filteredData)
                {
                    if(locations.contains(QString::fromStdString(water.getLocation())))
                    {
                        tempFilter.push_back(water);
                    }
                }
                LOGD(QString("maxLocations %1, locations.size() %2").arg(maxLocations).arg(locations.size()));
                break;
            }
            case Filter::FilterType::MATERIALS_MAX:
            {
                int maxMaterials = filter.value().toInt();
                if(maxMaterials == 0)
                {
                    break;
                }
                QStringList materials;
                for (const auto &water : m_filteredData)
                {
                    if(!materials.contains(QString::fromStdString(water.getDeterminand())))
                    {
                        materials.append(QString::fromStdString(water.getDeterminand()));
                    }

                    if(materials.size() >= maxMaterials)
                    {
                        break;
                    }
                }

                for (const auto &water : m_filteredData)
                {
                    if(materials.contains(QString::fromStdString(water.getDeterminand())))
                    {
                        tempFilter.push_back(water);
                    }
                }
                LOGD(QString("maxMaterials %1, materials.size() %2").arg(maxMaterials).arg(materials.size()));
                break;
            }
            default:
                break;
            }
            LOGD(QString("After filter %1, m_filteredData.size() %2").arg((int)filter.type()).arg(tempFilter.size()));
            m_filteredData = tempFilter;
            tempFilter.clear();
        }
    }
    LOGD(QString("m_filteredData.size() %1").arg(m_filteredData.size()));
    emit rawDataReady(m_filteredData);
    takePollutantOverviewData();
    takeEnvironmentalLitterIndicatorsData();
    takeFluorinatedCompoundsData();
    takeComplianceDashboardData();
    takePOPsData();
    setIsFilteredChanged(false);
}
bool DataHandler::loading()
{
    LOGD(QString("Loading data with file %1").arg(m_filename));
    std::ifstream file(m_filename.toStdString());
    size_t rowCount = 0;
    std::string line;
    while (std::getline(file, line))
    {
        ++rowCount;
    }
    if (rowCount == 0)
    {
        SET_VALUE(CSV_FILE_PATH, "");
        m_filename = "";
        emit handling(SHOW_PROGESS_VALUE);
        emit processingMessage("No data found in the file or file is empty");
        QThread::msleep(3000);
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
            row["sample.samplingPoint.easting"].get<double>(),
            row["sample.samplingPoint.northing"].get<double>(),
            QString::fromStdString(row["sample.isComplianceSample"].get<std::string>()).toLower() == "true"};
        m_rootData.push_back(water);
        if (!locations.contains(QString::fromStdString(water.getLocation())))
        {
            locations.append(QString::fromStdString(water.getLocation()));
        }
        if (!materials.contains(QString::fromStdString(water.getDeterminand())))
        {
            materials.append(QString::fromStdString(water.getDeterminand()));
        }
        emit handling((int)(count++ * 100 / rowCount));
        if (!m_isRunning)
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
void DataHandler::addFilter(const Filter &filter)
{
    LOG();
    switch (filter.type())
    {
    case Filter::FilterType::LOCATIONS_SET:
    case Filter::FilterType::LOCATIONS:
    case Filter::FilterType::LOCATIONS_CONTAIN:
    {
        for (int i = 0; i < m_filters.size(); ++i)
        {
            if (m_filters[i].type() == Filter::FilterType::LOCATIONS ||
                m_filters[i].type() == Filter::FilterType::LOCATIONS_CONTAIN ||
                m_filters[i].type() == Filter::FilterType::LOCATIONS_SET)
            {
                m_filters.removeAt(i);
                break;
            }
        }
        break;
    }
    case Filter::FilterType::MATERIALS:
    case Filter::FilterType::MATERIALS_CONTAIN:
    case Filter::FilterType::MATERIALS_SET:
    {
        for (int i = 0; i < m_filters.size(); ++i)
        {
            if (m_filters[i].type() == Filter::FilterType::MATERIALS ||
                m_filters[i].type() == Filter::FilterType::MATERIALS_CONTAIN ||
                m_filters[i].type() == Filter::FilterType::MATERIALS_SET)
            {
                m_filters.removeAt(i);
                break;
            }
        }
        break;
    }
    case Filter::FilterType::TIME:
    {
        for (int i = 0; i < m_filters.size(); ++i)
        {
            if (m_filters[i].type() == Filter::FilterType::TIME)
            {
                m_filters.removeAt(i);
                break;
            }
        }
        break;
    }
    default:
        break;
    }
    m_filters.append(filter);
    setIsFilteredChanged(true);
}
void DataHandler::removeFilter(const Filter &filter)
{
    LOG();
    m_filters.removeAll(filter);
}
void DataHandler::takeGeographicalData() {}
