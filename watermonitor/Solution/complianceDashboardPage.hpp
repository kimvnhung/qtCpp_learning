#pragma once

#include <QWidget>

class QPushButton;

/**
 * @brief Template for an information page.
 */
class ComplianceDashboardPage : public QWidget {
    Q_OBJECT

public:
    ComplianceDashboardPage(QWidget* parent = nullptr);

signals:
    void goBack();

private:
    QPushButton* backButton;
};
