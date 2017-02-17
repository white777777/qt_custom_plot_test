#include "mainwindow.h"
#include <QApplication>

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPainter>

#include "filedatasource.h"
#include "interfaces.h"
#include "qtdrawer.h"
#include "drawables.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    QString fileName = "/mnt/windows/Users/user/Documents/projects/QtTest/SampleFiles/MillionSamples WithTail.ssd";

    if (!fileName.isEmpty()) {
        try
        {
            QTDrawer* drawer  = new QTDrawer();
            auto dataSource = std::make_shared<FileDataSource>(fileName.toStdString());
            drawer->AddDrawable(std::make_shared<Graph>(dataSource));
            drawer->AddDrawable(std::make_shared<Legend>());
            drawer->AddDrawable(std::make_shared<Axes>());
            DataInfo dataInfo = dataSource->GetDataInfo();
            drawer->Zoom(dataInfo.limits);
            drawer->show();
        }
        catch(std::exception &e)
        {
            QMessageBox mb;
            mb.setText(e.what());
            mb.exec();
        }
    }


    return a.exec();
}
