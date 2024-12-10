#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

#include "filtercombobox.h"

#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QComboBox;
class QLineEdit;


class SettingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SettingPanel(QWidget *parent = nullptr);

    void setCollapse(bool collapse);

public slots:
    void setMaterialFilter(const QStringList &materials);
    void setLocationsFilter(const QStringList &locations);

    void onPageChanged(const QString &pageName);

signals:
    void csvFileAvailable(const QString &filePath);

    void materialFilterChanged(const QStringList &materials);
    void locationFilterChanged(const QStringList &locations);
    void timeFilterChanged(int index);

private:
    QHBoxLayout *m_mainLayout;
    QWidget *m_content;
    QPushButton *m_settingButton;

    QComboBox *m_languageComboBox;
    QComboBox *m_regionComboBox;
    QComboBox *m_timeComboBox;
    QLineEdit *m_filePathEdit;

    bool m_isCollapsed = false;

    QString m_currentChartName;
    // For filter
    FilterComboBox *m_materialFilter;
    FilterComboBox *m_locationFilter;




private:
    void initializeUi();

    void setUpSettingButton();

    void setUpContent();
private slots:
    void onOpenFileClicked();

};

#endif // SETTINGPANEL_H
