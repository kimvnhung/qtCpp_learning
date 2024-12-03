#pragma once

#include <QWidget>

class QPushButton;

/**
 * @brief Template for an information page.
 */
class EnvironmentalLitterIndicatorsPage : public QWidget
{
    Q_OBJECT

public:
    explicit EnvironmentalLitterIndicatorsPage(QWidget *parent = nullptr);

signals:
    void goBack();

private:
    QPushButton *backButton;
};
