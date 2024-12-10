// COMP2811 Coursework 2: data model

#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include "dataHandler.hpp"

class QTDateTime;


class WaterModel: public QAbstractTableModel
{
  Q_OBJECT
  public:
    WaterModel(QObject *parent = nullptr, DataHandler *dataHandler = nullptr);
    bool hasData() const;

    int rowCount(const QModelIndex& index) const;
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;

public slots:
    void updateData(std::vector<Water> data);

  private:
    std::vector<Water> m_data;

};
