#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "utils.h"

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

private:
    Ui::MainWindow *ui;
    void initUI();
    void setEnabledWidget(bool enabled);

    FilterConfig *config = nullptr;
};

#endif // MAINWINDOW_H
