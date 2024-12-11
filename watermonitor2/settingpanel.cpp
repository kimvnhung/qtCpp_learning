#include "settingpanel.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>
#include <QSpinBox>

#include "common.h"
#include "filtercombobox.h"
#include "charts/chartholderbasewidget.h"

#define COLLAPSED_WIDTH 25
#define EXPANDED_WIDTH 200

#define COLLAPSED_ICON QIcon{":/images/setting.png"}
#define EXPANDED_ICON QIcon{":/images/back.png"}

SettingPanel::SettingPanel(QWidget *parent)
    : QWidget{parent}
    , m_currentChartName{PREVIEW_MODE_NAME}
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

void SettingPanel::setMaterialFilter(const QStringList &materials)
{
    m_materialFilter->addCheckableItems(materials);
}

void SettingPanel::setLocationsFilter(const QStringList &locations)
{
    m_locationFilter->addCheckableItems(locations);
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
    QVBoxLayout *languageLayout = new QVBoxLayout;
    languageLayout->setContentsMargins(0,0,0,0);
    languageLayout->setSpacing(5);
    m_languageComboBox->addItem(tr("English"), "en");
    m_languageComboBox->addItem(tr("Spanish"), "es");
    m_languageComboBox->addItem(tr("French"), "fr");
    m_languageComboBox->addItem(tr("German"), "de");
    // Set for text in the middle
    m_languageComboBox->setStyleSheet("QComboBox { text-align: center; }");

    QLabel *languageLabel = new QLabel("Language:");
    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(m_languageComboBox);

    contentLayout->addLayout(languageLayout);

    // Setup region combo box
    // m_regionComboBox = new QComboBox{m_content};
    // m_regionComboBox->addItem(tr("Region 1"), "r1");
    // m_regionComboBox->addItem(tr("Region 2"), "r2");
    // m_regionComboBox->addItem(tr("Region 3"), "r3");
    // contentLayout->addWidget(m_regionComboBox);

    // Set up location filter
    m_locationFilter = new FilterComboBox{m_content};
    QVBoxLayout *locationLayout = new QVBoxLayout;
    locationLayout->setContentsMargins(0,0,0,0);
    locationLayout->setSpacing(5);

    QLabel *locationLabel = new QLabel("Location:");
    locationLayout->addWidget(locationLabel);
    locationLayout->addWidget(m_locationFilter);
    connect(m_locationFilter, &FilterComboBox::checkedItemsChanged,this, &SettingPanel::locationFilterChanged);

    QLabel *maxLocationLabel = new QLabel("Max locations:");
    QSpinBox *maxLocationSpinBox = new QSpinBox{m_content};
    connect(maxLocationSpinBox, &QSpinBox::valueChanged,this, &SettingPanel::maxLocationsChanged);
    maxLocationSpinBox->setMinimum(-1);
    maxLocationSpinBox->setValue(10);
    // Disable editing
    // maxLocationSpinBox->setReadOnly(true);
    QTimer::singleShot(500, [this](){
        emit maxLocationsChanged(10);
    });

    QHBoxLayout *maxLocationLayout = new QHBoxLayout;
    maxLocationLayout->addWidget(maxLocationLabel);
    maxLocationLayout->addWidget(maxLocationSpinBox);
    locationLayout->addLayout(maxLocationLayout);

    contentLayout->addLayout(locationLayout);

    // Set up material filter
    m_materialFilter = new FilterComboBox{m_content};
    QVBoxLayout *materialLayout = new QVBoxLayout;
    materialLayout->setContentsMargins(0,0,0,0);
    materialLayout->setSpacing(5);

    QLabel *materialLabel = new QLabel("Material:");
    materialLayout->addWidget(materialLabel);
    materialLayout->addWidget(m_materialFilter);
    connect(m_materialFilter, &FilterComboBox::checkedItemsChanged,this, &SettingPanel::materialFilterChanged);

    QLabel *maxMaterialLabel = new QLabel("Max materials:");
    QSpinBox *maxMaterialSpinBox = new QSpinBox{m_content};
    connect(maxMaterialSpinBox, &QSpinBox::valueChanged,this, &SettingPanel::maxMaterialsChanged);
    maxMaterialSpinBox->setMinimum(-1);
    maxMaterialSpinBox->setValue(10);
    // Disable editing
    // maxMaterialSpinBox->setReadOnly(true);
    QTimer::singleShot(500, [this](){
        emit maxMaterialsChanged(10);
    });

    QHBoxLayout *maxMaterialLayout = new QHBoxLayout;
    maxMaterialLayout->addWidget(maxMaterialLabel);
    maxMaterialLayout->addWidget(maxMaterialSpinBox);
    materialLayout->addLayout(maxMaterialLayout);

    contentLayout->addLayout(materialLayout);

    // Setup time combo box
    m_timeComboBox = new QComboBox{m_content};
    connect(m_timeComboBox, &QComboBox::currentIndexChanged, this, &SettingPanel::timeFilterChanged);
    m_timeComboBox->addItem("All Time");
    m_timeComboBox->addItem("Last Week");
    m_timeComboBox->addItem("Last Month");
    m_timeComboBox->addItem("Last 3 Months");
    m_timeComboBox->addItem("Last 6 Months");
    m_timeComboBox->addItem("Last Year");


    QVBoxLayout *timeLayout = new QVBoxLayout;
    timeLayout->setContentsMargins(0,0,0,0);
    timeLayout->setSpacing(5);
    QLabel *timeLabel = new QLabel("Time:");
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(m_timeComboBox);
    contentLayout->addLayout(timeLayout);

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

    QPushButton *reloadButton = new QPushButton("Reload", this);
    connect(reloadButton, &QPushButton::clicked, this, &SettingPanel::onReloadClicked);

    openFileButtonLayout->addStretch();
    openFileButtonLayout->addWidget(openFileButton);
    openFileButtonLayout->addWidget(reloadButton);

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
        emit csvFileAvailable(filePath);
    }
}

void SettingPanel::onReloadClicked()
{
    if(!m_filePathEdit->text().isEmpty())
        emit csvFileAvailable(m_filePathEdit->text());
    else
    {
        QMessageBox msgBox;
        msgBox.setText("The csv file path is empty. Select file path firstly!");
        msgBox.exec();
    }

}

void SettingPanel::onPageChanged(const QString &pageName)
{
    LOGD(pageName);
}
