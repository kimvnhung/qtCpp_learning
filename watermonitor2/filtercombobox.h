#ifndef FILTERCOMBOBOX_H
#define FILTERCOMBOBOX_H

#include <QComboBox>
#include <QWidget>

class FilterComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit FilterComboBox(QWidget *parent = nullptr);
    // Add items with checkable property
    void addCheckableItems(const QStringList& items);
    // Get all checked items
    QStringList checkedItems() const;
signals:
    void checkedItemsChanged(const QStringList& checkedItems);

private slots:
    // Filter items based on user input
    void filterItems(const QString& text);
    void handleActivatedItem(int index);
private:
    void setAllItemsCheckState(Qt::CheckState state);
};

#endif // FILTERCOMBOBOX_H
