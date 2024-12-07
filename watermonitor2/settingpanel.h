#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

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

private:
    QHBoxLayout *m_mainLayout;
    QWidget *m_content;
    QPushButton *m_settingButton;

    QComboBox *m_languageComboBox;
    QComboBox *m_regionComboBox;
    QComboBox *m_timeComboBox;
    QLineEdit *m_filePathEdit;

    bool m_isCollapsed = false;

private:
    void initializeUi();

    void setUpSettingButton();

    void setUpContent();
private slots:
    void onOpenFileClicked();

};

#endif // SETTINGPANEL_H
