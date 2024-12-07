#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QTranslator>

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

private:
    QLabel *appTitleLabel;
    QComboBox *languageSelector;
    QPushButton *navButtonPollutantOverview;
    QPushButton *navButtonPOPs;
    QPushButton *navButtonEnvLitterIndicators;
    QPushButton *navButtonFluorinatedCompounds;
    QPushButton *navButtonCompliance;

    QVBoxLayout *mainLayout;
    QTranslator translator; // Declaring translator as a member variable

    QWidget *createPollutantCard(const QString &title, const QString &summary, const QString &color);
    void setupHeader();
    void setupMainContent();
    void setupFooter();
    void retranslateUi();
    bool loadLanguage(const QString &languageCode);
};

#endif // DASHBOARD_H
