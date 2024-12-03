#pragma once

#include <QWidget>

class QPushButton;

/**
 * @brief Template for an information page.
 */
class FluorinatedCompoundsPage : public QWidget {
    Q_OBJECT

public:
    explicit FluorinatedCompoundsPage(QWidget* parent = nullptr);

signals:
    void goBack();

private:
    QPushButton* backButton;
};
