#include "filtercombobox.h"

#include <QLineEdit>
#include <QStandardItemModel>
#include <QTimer>
#include "common.h"

FilterComboBox::FilterComboBox(QWidget *parent)
    : QComboBox{parent}
{
    QStandardItemModel* model = new QStandardItemModel(this);
    setModel(model);
    setEditable(false);


    // Connect model's dataChanged signal to track changes in check state
    connect(this, &QComboBox::activated, this, &FilterComboBox::handleActivatedItem);

    // Catch on all Item check state changes
    connect(model, &QStandardItemModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
        if (roles.contains(Qt::CheckStateRole)) {
            emit checkedItemsChanged(checkedItems());
        }
    });
}

void FilterComboBox::addCheckableItems(const QStringList& items)
{
    //Reorder the items by alphabet
    QStringList sortedItems = items;
    std::sort(sortedItems.begin(), sortedItems.end());


    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model) return;

    // Add "Check All" option
    QStandardItem* checkAllItem = new QStandardItem("Check All");
    checkAllItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    model->appendRow(checkAllItem);

    // Add "Uncheck All" option
    QStandardItem* uncheckAllItem = new QStandardItem("Uncheck All");
    uncheckAllItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    model->appendRow(uncheckAllItem);

    // Add the actual items
    for (const QString& item : sortedItems) {
        QStandardItem* standardItem = new QStandardItem(item);
        standardItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        standardItem->setData(Qt::Checked, Qt::CheckStateRole);
        standardItem->setTextAlignment(Qt::AlignCenter);
        model->appendRow(standardItem);
    }
}

QStringList FilterComboBox::checkedItems() const
{
    QStringList checkedList;
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model) return checkedList;

    for (int i = 2; i < model->rowCount(); ++i) { // Skip "Check All" and "Uncheck All"
        QStandardItem* item = model->item(i);
        if (item->checkState() == Qt::Checked) {
            checkedList << item->text();
        }
    }
    return checkedList;
}

void FilterComboBox::handleActivatedItem(int index)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model) return;

    QStandardItem* item = model->item(index);
    if (!item) return;

    if (index == 0) {
        setAllItemsCheckState(Qt::Checked);
    } else if (index == 1) {
        setAllItemsCheckState(Qt::Unchecked);
    }else {
        Qt::CheckState state = (item->checkState() == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
        item->setCheckState(state);
    }

    // Emit the updated checked items signal
    emit checkedItemsChanged(checkedItems());
}

void FilterComboBox::setAllItemsCheckState(Qt::CheckState state)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model) return;

    for (int i = 2; i < model->rowCount(); ++i) { // Skip "Check All" and "Uncheck All"
        QStandardItem* item = model->item(i);
        if (item) {
            item->setCheckState(state);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        }
    }
}

