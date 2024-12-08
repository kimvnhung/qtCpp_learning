#include "settingpanel.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include "common.h"

#define COLLAPSED_WIDTH 25
#define EXPANDED_WIDTH 200

#define COLLAPSED_ICON QIcon{":/images/setting.png"}
#define EXPANDED_ICON QIcon{":/images/back.png"}

SettingPanel::SettingPanel(QWidget *parent)
    : QWidget{parent}
{
    initializeUi();
}

void SettingPanel::setCollapse(bool collapse)
{
    m_isCollapsed = collapse;

    SET_VALUE(SETTING_COLLAPSED, m_isCollapsed);

    if (m_isCollapsed) {
        setFixedWidth(COLLAPSED_WIDTH);
        m_settingButton->setIcon(COLLAPSED_ICON);
    } else {
        setFixedWidth(EXPANDED_WIDTH);
        m_settingButton->setIcon(EXPANDED_ICON);
    }
}

//private
void SettingPanel::initializeUi()
{
    // add QHBoxLayout to the SettingPanel
    m_mainLayout = new QHBoxLayout{this};
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);

    setUpSettingButton();
    setUpContent();

    setCollapse(GET_BOOL(SETTING_COLLAPSED));
    setStyleSheet("SettingPanel { background-color: #ffffff; }");
}

void SettingPanel::setUpSettingButton()
{
    // Set stylesheet for showing area
    // buttonContainer->setStyleSheet("background-color: #ff0000;");
    QVBoxLayout *buttonContainerLayout = new QVBoxLayout;

    // Remove magrins
    buttonContainerLayout->setContentsMargins(0,0,0,0);
    m_settingButton = new QPushButton{this};
    // set image to button
    m_settingButton->setFixedSize(COLLAPSED_WIDTH, COLLAPSED_WIDTH);
    // Set style on hover
    m_settingButton->setStyleSheet(R"(
        QPushButton {
            border: none;
            background-color: transparent;
            border-radius: 12px;
        }

        QPushButton:hover {
            background-color: #ffffff;
        }
)");
    m_settingButton->setIcon(QIcon{":/images/setting.png"});
    m_settingButton->setIconSize(QSize{(int)(COLLAPSED_WIDTH*0.6), (int)(COLLAPSED_WIDTH*0.6)});

    buttonContainerLayout->addWidget(m_settingButton);
    // Add vertical spacer to the layout
    buttonContainerLayout->addStretch();
    m_mainLayout->addLayout(buttonContainerLayout);

}

void SettingPanel::setUpContent()
{
    m_content = new QWidget{this};
    m_content->setStyleSheet("background-color: #ffffff;");
    m_content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *contentLayout = new QVBoxLayout{m_content};

    contentLayout->setSpacing(30);

    QLabel *label = new QLabel("Setting Panel");
    label->setAlignment(Qt::AlignHCenter);
    contentLayout->addWidget(label);

    // Set up language combo box
    m_languageComboBox = new QComboBox{m_content};
    m_languageComboBox->addItem(tr("English"), "en");
    m_languageComboBox->addItem(tr("Spanish"), "es");
    m_languageComboBox->addItem(tr("French"), "fr");
    m_languageComboBox->addItem(tr("German"), "de");
    // Set for text in the middle
    m_languageComboBox->setStyleSheet("QComboBox { text-align: center; }");
    contentLayout->addWidget(m_languageComboBox);

    // Setup region combo box
    m_regionComboBox = new QComboBox{m_content};
    m_regionComboBox->addItem(tr("Region 1"), "r1");
    m_regionComboBox->addItem(tr("Region 2"), "r2");
    m_regionComboBox->addItem(tr("Region 3"), "r3");
    contentLayout->addWidget(m_regionComboBox);

    // Setup time combo box
    m_timeComboBox = new QComboBox{m_content};
    m_timeComboBox->addItem(tr("Last Month"), "lm");
    m_timeComboBox->addItem(tr("Last Year"), "ly");
    contentLayout->addWidget(m_timeComboBox);

    // Add file path label
    QVBoxLayout *filePathLayout = new QVBoxLayout;
    filePathLayout->setContentsMargins(0,0,0,0);
    filePathLayout->setSpacing(5);
    QLabel *filePathLabel = new QLabel("File Path:");
    filePathLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    filePathLayout->addWidget(filePathLabel);

    m_filePathEdit = new QLineEdit(this);
    m_filePathEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_filePathEdit->setPlaceholderText("Select a file...");
    m_filePathEdit->setReadOnly(true);

    if(!GET_STRING(CSV_FILE_PATH).isEmpty()) {
        m_filePathEdit->setText(GET_STRING(CSV_FILE_PATH));
    }

    QHBoxLayout *openFileButtonLayout = new QHBoxLayout;
    QPushButton *openFileButton = new QPushButton("Open", this);
    connect(openFileButton, &QPushButton::clicked, this, &SettingPanel::onOpenFileClicked);
    openFileButtonLayout->addStretch();
    openFileButtonLayout->addWidget(openFileButton);

    filePathLayout->addWidget(m_filePathEdit);
    filePathLayout->addLayout(openFileButtonLayout);
    filePathLayout->addStretch();


    contentLayout->addLayout(filePathLayout);
    contentLayout->addStretch();

    // Add m_content to the first widget of the layout
    m_mainLayout->insertWidget(0,m_content);
}

void SettingPanel::onOpenFileClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath());
    if (!filePath.isEmpty()) {
        m_filePathEdit->setText(filePath);
        SET_VALUE(CSV_FILE_PATH, filePath);
    }
}
