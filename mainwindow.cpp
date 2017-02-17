#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPainter>

#include "filedatasource.h"
#include "interfaces.h"
#include "qtdrawer.h"
#include "drawables.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
    //        tr("Sample files (*.ssd);;"));

    QString fileName = "/mnt/windows/Users/user/Documents/projects/QtTest/SampleFiles/";

    if (!fileName.isEmpty()) {
        try
        {
            QTDrawer* drawer  = new QTDrawer(this->centralWidget());
            drawer->AddDrawable(std::make_shared<Graph>(std::make_shared<FileDataSource>(fileName.toStdString())));
            drawer->show();
        }
        catch(std::exception &e)
        {
            QMessageBox mb;
            mb.setText(e.what());
            mb.show();
        }
    }
}
