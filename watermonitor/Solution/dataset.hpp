

#pragma once

#include <vector>
#include "water.hpp"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QUrl>
#include <QDebug>
#include <queue>

class Water;

class WaterDataset
{
  public:
    WaterDataset(const std::string& filename) { loadData(filename); }
    WaterDataset(std::vector<Water> data) : data(data) {}
    WaterDataset() : networkManager(new QNetworkAccessManager()) {}
    ~WaterDataset() { delete networkManager; }
    void loadData(const std::string& filename);
    void fetchLatLong(const QString& id, Water* water);
    int size() const { return data.size(); }
    Water operator[](int index) const { return data.at(index); }
    std::vector<Water> data;
    

  private:
    void checkDataExists() const;
    QNetworkAccessManager* networkManager; // Single instance
    std::queue<std::pair<QString, Water*>> requestQueue; // Queue for requests
    bool processingQueue = false; // To check if requests are being processed
    void processNextRequest(); // Helper to process the queue
};
