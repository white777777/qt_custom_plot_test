#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPainter>
#include <QSettings>

#include "filedatasource.h"
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
    const QString DEFAULT_DIR_KEY("Last ssd dir");
    QSettings MySettings;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), MySettings.value(DEFAULT_DIR_KEY).toString(),
            tr("Sample files (*.ssd);;"));

    if (!fileName.isEmpty()) {
        MySettings.setValue(DEFAULT_DIR_KEY, fileName);
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

void MainWindow::on_actionClear_triggered()
{
    _drawer->Clear();
}

void MainWindow::on_actionSave_Image_triggered()
{
    QImage img =_drawer->GetImage();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("Png image files (*.png);;"));

    if (!fileName.isEmpty())
    {
        img.save(fileName);
    }
}

void MainWindow::on_actionRemove_last_added_triggered()
{
    _drawer->RemoveLastGraph();
}
