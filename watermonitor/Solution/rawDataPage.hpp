#pragma once

#include <QWidget>
#include <QStackedWidget>

class QPushButton;
class QGridLayout;
class QWidget;
class CreateFilters;

/**
 * @brief Template for an information page.
 */
class RawDataPage : public QWidget {
    Q_OBJECT

public:
    explicit RawDataPage(QWidget* parent = nullptr, QStackedWidget* pageStack = nullptr);
    QGridLayout* mainLayout;
    CreateFilters* timePeriodFilter;
    
signals:
    void goBack();
    void loadCSV(); 

private:
    QPushButton* backButton;

};
