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
    a.setApplicationName("Simple Graph Drawer");
    a.setOrganizationName("Radik Kurbanov");
    a.setOrganizationDomain("ksugltronwhite@gmail.com");
    MainWindow w;
    w.show();
    return a.exec();
}
