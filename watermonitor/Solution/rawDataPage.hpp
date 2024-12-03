#pragma once

#include <QWidget>

class QPushButton;
class QGridLayout;
class QWidget;

/**
 * @brief Template for an information page.
 */
class RawDataPage : public QWidget {
    Q_OBJECT

public:
    explicit RawDataPage(QWidget* parent = nullptr);
    QGridLayout* mainLayout;

signals:
    void goBack();
    void loadCSV(); 

private:
    QPushButton* backButton;

};
