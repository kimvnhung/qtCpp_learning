#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "utils.h"
#include "imagepreviewwidget.h"

using namespace Utils;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectBtn_clicked();
    void on_folderPathEdt_textChanged(const QString &arg1);
    void on_imagesListView_clicked(const QModelIndex &index);
    void on_addBtn_clicked();
    void on_deleteBtn_clicked();
    void on_tagCbb_currentIndexChanged(int index);
    void on_targetTagCbb_currentIndexChanged(int index);
    void on_targetTagListView_clicked(const QModelIndex &index);
    void on_exportBtn_clicked();
    void on_deleteFImageBtn_clicked();
    void on_deleteTImageBtn_clicked();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    void initUI();
    void setEnabledWidget(bool enabled);

    FilterConfig config;

    // Define QStringListModel for images showing in list view
    QStringListModel *fromModel;
    QStringListModel *targetModel;

    QString currentImagePath;
    QString showingImagePath;
    int fSelectedIndex = -1;
    int tSelectedIndex = -1;
    enum SelectedType {
        UNDEFINED,
        FSELECTED,
        TSELECTED
    };

    SelectedType lastSelection = UNDEFINED;
    ImagePreviewWidget *previewWidget;

    void refreshView();
};

#endif // MAINWINDOW_H
