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

public slots:
    void setMaterialFilter(const QStringList &materials);
    void setLocationsFilter(const QStringList &locations);

signals:
    void csvFileAvailable(const QString &filePath);

    void materialFilterChanged(const QStringList &materials);
    void locationFilterChanged(const QStringList &locations);
    void timeFilterChanged(int index);

    void maxLocationsChanged(int maxLocations);
    void maxMaterialsChanged(int maxMaterials);

private:
    QHBoxLayout *m_mainLayout;
    QWidget *m_content;

    QComboBox *m_languageComboBox;
    QComboBox *m_regionComboBox;
    QComboBox *m_timeComboBox;
    QLineEdit *m_filePathEdit;

    QString m_currentChartName;
    // For filter
    FilterComboBox *m_materialFilter;
    FilterComboBox *m_locationFilter;




private:
    void initializeUi();

    void setUpContent();
private slots:
    void onOpenFileClicked();
    void onReloadClicked();
};

#endif // SETTINGPANEL_H
