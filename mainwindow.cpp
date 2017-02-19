#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPainter>

#include "filedatasource.h"
#include "interfaces.h"
#include "drawables.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _drawer = new QTDrawer();
    _drawer->AddDrawable(std::make_shared<Legend>());
    _drawer->AddDrawable(std::make_shared<Axes>());

    this->setCentralWidget(_drawer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QColor GenColor(int colorIndex)
{
    int h=int(100*colorIndex)%360;
    return QColor::fromHsv(h, 255, 255);
}

void MainWindow::on_actionAdd_Graph_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Sample files (*.ssd);;"));

    if (!fileName.isEmpty()) {
        try
        {

            auto fds = std::make_shared<FileDataSource>(fileName.toStdString());
            if(!fds->GetErrInfo().empty())
            {
                QMessageBox mb;
                mb.setText(fds->GetErrInfo().c_str());
                mb.exec();
            }
            auto graph = std::make_shared<Graph>(fds);

            graph->SetColor(GenColor(++colorIndex));

            _drawer->AddDrawable(graph);
            _drawer->ResetLimits();
        }
        catch(std::exception &e)
        {
            QMessageBox mb;
            mb.setText(e.what());
            mb.exec();
        }
    }

}
