#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>

class StatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = nullptr);

signals:
};

#endif // STATUSBAR_H
