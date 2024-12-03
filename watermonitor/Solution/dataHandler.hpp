#pragma once

#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

#include <QObject>
#include <vector>
#include "water.hpp"
#include "dataset.hpp"

class DataHandler : public QObject
{
    Q_OBJECT
public:
    DataHandler(QObject *parent = nullptr);
    ~DataHandler();

    WaterDataset getDataset() const;

signals:
    void dataReady();
public slots:
    void loadData(const std::string &filename);

private:
    std::vector<Water> data;
};

#endif // DATAHANDLER_HPP
