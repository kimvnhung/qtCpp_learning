// COMP2811 Coursework 2: data model

#include "model.hpp"
#include "common.hpp"

WaterModel::WaterModel(QObject *parent, DataHandler *dataHandler)
    : QAbstractTableModel(parent)
    , dataHandler(dataHandler)
{
    connect(dataHandler, &DataHandler::dataReady, this, &WaterModel::updateData);
}

void WaterModel::updateFromFile(const QString &filename)
{
    LOG();
    dataHandler->loadData(filename.toStdString());
}

bool WaterModel::hasData() const
{
    return !m_data.empty();
}

int WaterModel::rowCount(const QModelIndex &index) const
{
    return m_data.size();
}

int WaterModel::columnCount(const QModelIndex &index) const
{
    return 9;
}

QVariant WaterModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  if (role == Qt::TextAlignmentRole)
  {
    return int(Qt::AlignRight | Qt::AlignVCenter);
  }
  else if (role == Qt::DisplayRole)
  {
    Water q = m_data[index.row()];
    switch (index.column())
    {
    case 0:
      return QVariant(q.getID().c_str());
    case 1:
      return QVariant(q.getLocation().c_str());
    case 2:
      return QVariant(q.getTime().c_str());
    case 3:
      return QVariant(q.getDeterminand().c_str());
    case 4:
      return QVariant(q.getResult());
    case 5:
      return QVariant(q.getUnit().c_str());
    case 6:
      return QVariant(q.getWaterType().c_str());
    case 7:
      return QVariant(q.getLat());
    case 8:
      return QVariant(q.getLong());
    }
  }

  return QVariant();
}

QVariant WaterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
  {
    return QVariant();
  }

  if (orientation == Qt::Vertical)
  {
    return QVariant(section + 1);
  }

  switch (section)
  {
  case 0:
    return QString("ID");
  case 1:
    return QString("Location");
  case 2:
    return QString("Time");
  case 3:
    return QString("Determinand");
  case 4:
    return QString("Result");
  case 5:
    return QString("Unit");
  case 6:
    return QString("Water Type");
  case 7:
    return QString("Latitude");
  case 8:
    return QString("Longitude");
  default:
    return QVariant();
  }
}

void WaterModel::updateData()
{
  LOG();
  beginResetModel();
    m_data = dataHandler->getData();
  endResetModel();
}
