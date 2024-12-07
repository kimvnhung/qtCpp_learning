#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QWidget>

class QProgressBar;
class QLabel;
class QVBoxLayout;

class StatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = nullptr);

    void setValue(int value);
    void setText(const QString &text);

private:
    QProgressBar *m_progressBar;
    QLabel *m_overlayText;

    QVBoxLayout *m_mainLayout;

private:
    void initializeUi();

    void setUpProgressBar();
    void setUpOverlayText();

};

#endif // STATUSBAR_H
