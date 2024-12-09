#ifndef GENERALDASHBOARD_H
#define GENERALDASHBOARD_H

#include "overviewchartwidget.h"
#include "settingpanel.h"
#include "statusbar.h"
#include "handlers/dataHandler.hpp"

#include <QGridLayout>
#include <QWidget>

class GeneralDashboard : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralDashboard(QWidget *parent = nullptr);
    ~GeneralDashboard();

public slots:
    void onProcessing(int percent);
    void setProcessingMessage(QString message);

private:
    void initializeUi();
    void setUpHeader();
    void setUpSettingPanel();
    void setUpChartPanel();
    void setUpStatusBar();
    void setUpPreventDialog();

    void setUpDataHandler();
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_contentLayout;
    SettingPanel *m_settingPanel;
    OverviewChartWidget *m_chartPanel;
    StatusBar *m_statusBar;
    DataHandler *m_dataHandler;

    QDialog *m_dialog;

};

#endif // GENERALDASHBOARD_H
