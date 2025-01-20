#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "common.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeyEvent>

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
    model = new QStringListModel(this);
    ui->imagesListView->setModel(model);

    // Make ui.imageDisplayLb expanding follow parent size but keep aspect
    ui->imageDisplayLb->setScaledContents(true);


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
    // Check if the path is valid
    if (!QDir(arg1).exists())
    {
        setEnabledWidget(false);
        return;
    }

    // Check if config.json exists in the selected folder
    QString filePath = arg1 + "/config.json";
    if (!QFile::exists(filePath))
    {
        // Create a new config file
        config = FilterConfig(arg1);
    }
    else
    {
        config = FilterConfig::importFromJson(filePath);
    }

    config.updateImages();

    if (config.currentPath().isEmpty())
    {
        SHOW_MESSAGE_BOX("Warning!", "No data found in the selected folder")
        setEnabledWidget(false);
        return;
    }

    setEnabledWidget(true);

    // Save the last path
    SET_LAST_PATH(arg1);

    // Update the tag combobox
    ui->tagCbb->clear();
    ui->tagCbb->addItems(config.getAllTags());

    // Update the list view QStringListModel
    model->setStringList(config.getImages("All"));
    ui->imagesListView->update();
}

void MainWindow::on_imagesListView_clicked(const QModelIndex &index)
{
    // Load image from the selected index
    currentImagePath = config.currentPath() + "/" + model->data(index).toString();

    // Load the image
    QPixmap pixmap(currentImagePath);

    // Scale the pixmap to fit the label but keep aspect ratio
    QSize size = ui->imageDisplayLb->size();
    QPixmap scaledPixmap = pixmap.scaled(size, Qt::KeepAspectRatio);

    // Set the scaled pixmap to the QLabel
    ui->imageDisplayLb->setPixmap(scaledPixmap);

}


void MainWindow::on_addBtn_clicked()
{
    // Open dialog for input new tag
    bool ok;
    QString tag = QInputDialog::getText(
        this,
        "Add new tag",
        "Tag name:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (!ok || tag.isEmpty())
        return;

    // Add new tag to the combobox
    ui->tagCbb->addItem(tag);

    // Add new tag to the config
    config.addTag(tag);

    if(config.getAllTags().size() > 2)
    {
        ui->targetTagCbb->clear();
        ui->targetTagCbb->addItems(config.getAllTags());
        ui->targetTagCbb->removeItem(0);
    }
    else
    {
        ui->targetTagCbb->clear();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        qDebug() << "Space key released!";
        // Handle space release logic here
        // Get image name from currentImagePath
        QString imageName = currentImagePath.split("/").last();
        // Get current tag from targetTagCbb
        QString currentTag = ui->targetTagCbb->currentText();
        // Add the image to the current tag
        config.addImage(imageName, currentTag);
    } else {
        QMainWindow::keyReleaseEvent(event);
    }
}

