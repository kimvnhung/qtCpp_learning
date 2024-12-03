// COMP2811 Coursework 1 sample solution: QuakeDataset class

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"

using namespace std;

void WaterDataset::fetchLatLong(const QString &id, Water *water)
{

    QNetworkRequest request{QUrl(id)};
    QNetworkReply *reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [this, reply, water]()
                     {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();
                if (jsonObj.contains("items") && jsonObj["items"].isArray()) {
                    QJsonArray items = jsonObj["items"].toArray();
                    if (!items.isEmpty()) {
                        QJsonObject firstItem = items[0].toObject();
                        if (firstItem.contains("sample") && firstItem["sample"].isObject()) {
                            QJsonObject sample = firstItem["sample"].toObject();
                            if (sample.contains("samplingPoint") && sample["samplingPoint"].isObject()) {
                                QJsonObject samplingPoint = sample["samplingPoint"].toObject();
                                if (samplingPoint.contains("lat") && samplingPoint.contains("long")) {
                                    double latitude = samplingPoint["lat"].toDouble();
                                    double longitude = samplingPoint["long"].toDouble();
                                    water->setLatLong(latitude, longitude);
                                }
                                
                            }
                        }
                    }
                }
            }
        }

        reply->deleteLater();
        processNextRequest(); });
}

void WaterDataset::processNextRequest()
{
    if (requestQueue.empty())
    {
        processingQueue = false;

        return;
    }

    processingQueue = true;
    auto [id, water] = requestQueue.front();
    requestQueue.pop();

    fetchLatLong(id, water);
}

void WaterDataset::loadData(const std::string &filename)
{
    if (processingQueue)
    {
        std::cerr << "Cannot load data while processing requests. Wait until processing completes." << std::endl;
        return;
    }

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
        requestQueue.emplace(QString::fromStdString(row["@id"].get<std::string>()), &data.back());
    }

    processNextRequest();
}

void WaterDataset::checkDataExists() const
{
    if (size() == 0)
    {
        throw std::runtime_error("Dataset is empty!");
    }
}
