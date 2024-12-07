// COMP2811 Coursework 2: data model

#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include "dataset.hpp"
#include "dataHandler.hpp"

class QTDateTime;


class WaterModel: public QAbstractTableModel
{
  Q_OBJECT
  public:
    WaterModel(QObject *parent = nullptr, DataHandler *dataHandler = nullptr);
    void updateFromFile(const QString&);
    bool hasData() const;

    int rowCount(const QModelIndex& index) const;
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;

  public slots:
    void updateData();

  private:
    WaterDataset dataset;
    DataHandler *dataHandler;

    std::vector<Water> m_data;

};
