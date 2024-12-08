#include "filtercombobox.h"

#include <QLineEdit>
#include <QStandardItemModel>

FilterComboBox::FilterComboBox(QWidget *parent)
    : QComboBox{parent}
{
    QStandardItemModel* model = new QStandardItemModel(this);
    setModel(model);
    setEditable(true);

    QLineEdit* lineEdit = this->lineEdit();
    connect(lineEdit, &QLineEdit::textChanged, this, &FilterComboBox::filterItems);

    // Connect model's dataChanged signal to track changes in check state
    connect(this, &QComboBox::activated, this, &FilterComboBox::handleActivatedItem);

}

void FilterComboBox::addCheckableItems(const QStringList& items)
{
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
    for (const QString& item : items) {
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

void FilterComboBox::filterItems(const QString& text)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model) return;

    for (int i = 2; i < model->rowCount(); ++i) { // Skip "Check All" and "Uncheck All"
        QStandardItem* item = model->item(i);
        bool match = item->text().contains(text, Qt::CaseInsensitive);
        item->setEnabled(match);
    }
}

void FilterComboBox::handleActivatedItem(int index)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (!model) return;

    QStandardItem* item = model->item(index);
    if (!item) return;

    if (item->text() == "Check All") {
        setAllItemsCheckState(Qt::Checked);
    } else if (item->text() == "Uncheck All") {
        setAllItemsCheckState(Qt::Unchecked);
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
        }
    }
}


