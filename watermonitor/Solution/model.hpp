// COMP2811 Coursework 2: data model

#pragma once

#include <QAbstractTableModel>
#include "dataset.hpp"
#include "dataHandler.hpp"

class WaterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    WaterModel(QObject *parent = nullptr);
  void updateFromFile(const QString &);
  bool hasData() const { return dataset.size() > 0; }

  int rowCount(const QModelIndex &index) const { return dataset.size(); }
  int columnCount(const QModelIndex &index) const { return 9; }
  QVariant data(const QModelIndex &, int) const;
  QVariant headerData(int, Qt::Orientation, int) const;

signals:
  void loadData(const std::string &filename);
public slots:
    void onDataReady();
private:
  WaterDataset dataset;
    DataHandler dataHandler;
};
