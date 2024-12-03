#include "dataHandler.hpp"

#include "csv.hpp"

DataHandler::DataHandler(QObject *parent) : QObject(parent)
{
}

DataHandler::~DataHandler()
{
}

WaterDataset DataHandler::getDataset() const
{
    return data;
}

void DataHandler::loadData(const std::string &filename)
{
    csv::CSVReader reader(filename);
    data.clear();

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
        };

        data.push_back(water);
    }
    emit dataReady();
}

