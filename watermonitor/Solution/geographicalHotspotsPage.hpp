#pragma once

#include <QWidget>

class QPushButton;

/**
 * @brief Template for an information page.
 */
class GeographicalHotspotsPage: public QWidget {
    Q_OBJECT

public:
    explicit GeographicalHotspotsPage(QWidget* parent = nullptr);

signals:
    void goBack();

private:
    QPushButton* backButton;
};
