#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "projecttree.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ProjectTree *projectTree;
    QString projectPath="/home";

private slots:
    void on_actionOpen_Project_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
