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

void MainWindow::refreshView()
{
    if (config.getAllTags().size() < 1)
        return;

    if (config.getAllTags().size() == 1)
    {
        ui->targetTagCbb->clear();
        ui->targetTagCbb->setEnabled(false);
        ui->deleteBtn->setEnabled(true);

        targetModel->setStringList({});
        ui->targetTagListView->update();
        ui->targetTagListView->setEnabled(false);

        config.updateImages();
        fromModel->setStringList(config.getImages("All"));
        ui->imagesListView->update();
        return;
    }

    ui->deleteBtn->setEnabled(true);
    ui->targetTagListView->setEnabled(true);

    targetModel->setStringList(config.getImages(ui->targetTagCbb->currentText()));
    ui->targetTagListView->update();
}

void MainWindow::initUI()
{
    currentImagePath = "";
    showingImagePath = "";

    fromModel = new QStringListModel(this);
    ui->imagesListView->setModel(fromModel);

    targetModel = new QStringListModel(this);
    ui->targetTagListView->setModel(targetModel);

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
    fromModel->setStringList(config.getImages("All"));
    ui->imagesListView->update();
    if (config.getAllTags().size() >= 2)
    {
        ui->targetTagCbb->clear();
        ui->targetTagCbb->addItems(config.getAllTags());
        ui->targetTagCbb->removeItem(0);
    }
}

void MainWindow::on_imagesListView_clicked(const QModelIndex &index)
{
    fSelectedIndex = index.row();
    // Load image from the selected index
    showingImagePath = config.currentPath() + "/" + fromModel->data(index).toString();

    // Load the image
    QPixmap pixmap(showingImagePath);

    // Scale the pixmap to fit the label but keep aspect ratio
    QSize size = ui->imageDisplayLb->size();
    QPixmap scaledPixmap = pixmap.scaled(size, Qt::KeepAspectRatio);

    // Set the scaled pixmap to the QLabel
    ui->imageDisplayLb->setPixmap(scaledPixmap);

    currentImagePath = showingImagePath;
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

    qDebug() << QString("newTags = [%1]").arg(config.getAllTags().join(","));

    if (config.getAllTags().count() >= 2)
    {
        qDebug() << __FUNCTION__ << "tags count = " << config.getAllTags().size();
        ui->targetTagCbb->clear();
        ui->targetTagCbb->addItems(config.getAllTags());
        ui->targetTagCbb->removeItem(0);
    }
    else
    {
        ui->targetTagCbb->clear();
    }

    config.sync();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
    {
        qDebug() << "Space key released!";
        // Handle space release logic here
        if (ui->tagCbb->currentText() == ui->targetTagCbb->currentText())
        {
            SHOW_MESSAGE_BOX("Error!", "Target tag must be different with from tag");
            return;
        }

        // Get image name from currentImagePath
        if (fSelectedIndex != -1 && currentImagePath == showingImagePath)
        {
            QString imageName = currentImagePath.split("/").last();
            // Get current tag from targetTagCbb
            QString currentTag = ui->targetTagCbb->currentText();
            // Add the image to the current tag
            config.addImage(imageName, currentTag);
            config.sync();
            refreshView();
        }
        else
        {
            SHOW_MESSAGE_BOX("Warning!", "Select from tag images");
        }
    }
    else
    {
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::on_deleteBtn_clicked()
{
    config.deleteTag(ui->tagCbb->currentText());
    ui->tagCbb->clear();
    ui->tagCbb->addItems(config.getAllTags());

    ui->targetTagCbb->clear();
    ui->targetTagCbb->addItems(config.getAllTags());
    ui->targetTagCbb->removeItem(0);

    config.sync();
}

void MainWindow::on_tagCbb_currentIndexChanged(int index)
{
    ui->fromTagLb->setText(ui->tagCbb->currentText());
    fromModel->setStringList(config.getImages(ui->tagCbb->currentText()));
    ui->imagesListView->update();
}

void MainWindow::on_targetTagCbb_currentIndexChanged(int index)
{
    ui->toTagLb->setText(ui->targetTagCbb->currentText());
    targetModel->setStringList(config.getImages(ui->targetTagCbb->currentText()));
    ui->targetTagListView->update();
}

void MainWindow::on_targetTagListView_clicked(const QModelIndex &index)
{
    tSelectedIndex = index.row();
    // Load image from the selected index
    showingImagePath = config.currentPath() + "/" + targetModel->data(index).toString();

    // Load the image
    QPixmap pixmap(showingImagePath);

    // Scale the pixmap to fit the label but keep aspect ratio
    QSize size = ui->imageDisplayLb->size();
    QPixmap scaledPixmap = pixmap.scaled(size, Qt::KeepAspectRatio);

    // Set the scaled pixmap to the QLabel
    ui->imageDisplayLb->setPixmap(scaledPixmap);
}

void MainWindow::on_exportBtn_clicked()
{
    QString lastExportPath = GET_LAST_EXPORT_PATH();
    // Open folder selection dialog
    QString selectedDir = QFileDialog::getExistingDirectory(
        this,                // Parent widget (can be nullptr)
        "Select Folder",     // Dialog title
        lastExportPath.isEmpty() ?
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) :
            lastExportPath, // Default directory (can be left empty)
        QFileDialog::ShowDirsOnly  // Show only directories
    );

    if (selectedDir.isEmpty())
        return;

    SET_LAST_EXPORT_PATH(selectedDir);

    auto tags = config.getAllTags();
    tags.removeAll("All");
    int success = 0;
    int total = 0;
    for (int i = 0; i < tags.size(); i++)
    {
        QString tag = tags[i];
        auto images = config.getImages(tag);
        foreach (QString image, images)
        {
            total++;
            QString imagePath = config.imagePath("All", image);
            if (copyFile2Folder(imagePath, selectedDir + "/" + tag))
                success++;
        }
    }

    SHOW_MESSAGE_BOX("Notice!", QString("Export success count %1/%2").arg(success).arg(total));
}

void MainWindow::on_deleteFImageBtn_clicked()
{
    if (ui->tagCbb->currentText() == "All" || fSelectedIndex == -1)
        return;

    config.deleteImage(fromModel->index(fSelectedIndex).data().toString(), ui->tagCbb->currentText());

    refreshView();
}

void MainWindow::on_deleteTImageBtn_clicked()
{
    if (config.getAllTags().size() <= 1 || tSelectedIndex == -1)
        return;

    config.deleteImage(targetModel->index(tSelectedIndex).data().toString(), ui->targetTagCbb->currentText());

    refreshView();
}
