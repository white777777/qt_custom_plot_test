#ifndef QTDRAWER_H
#define QTDRAWER_H

#include "interfaces.h"
#include <memory>
#include <vector>
#include <QWidget>
#include <QMouseEvent>
#include <algorithm>
class QTDrawer : public QWidget, public IDrawer
{
    Q_OBJECT
public:
    explicit QTDrawer(QWidget *parent = 0);

    //IDrawer
    DLimits GetDrawingLimits() override;
    std::vector<std::shared_ptr<IGraph>> GetGraphs() override;

    void mouseReleaseEvent ( QMouseEvent * event ) override;

    void AddDrawable(std::shared_ptr<IGraph> drawable);
    void AddDrawable(std::shared_ptr<IAxes> drawable);
    void AddDrawable(std::shared_ptr<ILegend> drawable);
    void Zoom(const DLimits & limits);
    void ResetLimits();
protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:

private:
    std::vector<std::shared_ptr<IGraph>> _graphs;
    std::vector<std::shared_ptr<IDrawable>> _overDrawables;
    DLimits _drawerInfo;
};

#endif // QTDRAWER_H
