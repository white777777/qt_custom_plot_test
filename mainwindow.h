#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtdrawer.h"
#include <QGridLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAdd_Graph_triggered();

    void on_actionClear_triggered();

    void on_actionSave_Image_triggered();

    void on_actionRemove_last_added_triggered();

private:
    Ui::MainWindow *ui;
    QTDrawer* _drawer;
    int colorIndex=0;
};

#endif // MAINWINDOW_H
