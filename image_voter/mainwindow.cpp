#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "common.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setEnabledWidget(bool enabled)
{
    ui->addBtn->setEnabled(enabled);
    ui->deleteBtn->setEnabled(enabled);
    ui->exportBtn->setEnabled(enabled);
    ui->tagCbb->setEnabled(enabled);
}

void MainWindow::initUI()
{
    QString oldFolder = GET_LAST_PATH();

    if (oldFolder.isEmpty())
    {
        SHOW_MESSAGE_BOX("Warning!", "No folder found")
        setEnabledWidget(false);
        return;
    }

    setEnabledWidget(true);
    ui->folderPathEdt->setText(oldFolder);
}

void MainWindow::on_selectBtn_clicked()
{
    // Open folder selection dialog
    QString selectedDir = QFileDialog::getExistingDirectory(
        this,                // Parent widget (can be nullptr)
        "Select Folder",     // Dialog title
        ui->folderPathEdt->text().isEmpty() ?
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) :
        ui->folderPathEdt->text(), // Default directory (can be left empty)
        QFileDialog::ShowDirsOnly  // Show only directories
    );

    if (selectedDir.isEmpty())
        return;

    ui->folderPathEdt->setText(selectedDir);
}

void MainWindow::on_folderPathEdt_textChanged(const QString &arg1)
{
    qDebug() << __FUNCTION__ << arg1;
}
