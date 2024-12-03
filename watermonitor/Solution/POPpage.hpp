#pragma once

#include <QWidget>
#include "ui_elements.hpp"

class QPushButton;
class QVBoxLayout;
class QLabel;
class QFrame;
class QLineEdit;
class QRect;
class QPushButton;

/**
 * @brief Template for an information page.
 */
class POPpage : public QWidget {
    Q_OBJECT
public:
    explicit POPpage(QWidget* parent = nullptr);

signals:
    void goBack();

private:
    QPushButton* backButton;

};
