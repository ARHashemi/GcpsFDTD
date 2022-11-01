#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDomDocument>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_Project_triggered()
{
    QTextStream out(stdout);
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
        projectPath, tr("FDTD files (*.FDTD);;XML files (*.xml);;Other (*.*)"));

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDomDocument *projectFile = new QDomDocument();
            if (projectFile->setContent(&file)) {
                projectTree = new ProjectTree(projectFile);//, ui->projectTree
                ui->projectTree->setModel(projectTree->projectTreeModel);
                ui->projectTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
                projectPath = filePath;
                out << projectPath << "\n";
            }
        }
    }
}

