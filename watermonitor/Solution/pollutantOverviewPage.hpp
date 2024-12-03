#pragma once

#include <QWidget>

class QPushButton;

/**
 * @brief Template for an information page.
 */
class PollutantOverviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit PollutantOverviewPage(QWidget *parent = nullptr);

signals:
    void goBack();

private:
    QPushButton *backButton;
};
