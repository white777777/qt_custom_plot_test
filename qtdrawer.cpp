#include "qtdrawer.h"
#include "drawables.h"

QTDrawer::QTDrawer(QWidget *parent) : QWidget(parent)
{
}

void QTDrawer::AddDrawable(std::shared_ptr<IGraph> drawable)
{
    _graphs.push_back(drawable);
}

void QTDrawer::AddDrawable(std::shared_ptr<IAxes> drawable)
{
    _overDrawables.push_back(drawable);
}

void QTDrawer::AddDrawable(std::shared_ptr<ILegend> drawable)
{
    _overDrawables.push_back(drawable);
}

void QTDrawer::Zoom(const DLimits & limits)
{
    _drawerInfo = limits;
    update();
}

DLimits QTDrawer::GetDrawingLimits()
{
    return _drawerInfo;
}

std::vector<std::shared_ptr<IGraph> > QTDrawer::GetGraphs()
{
    return _graphs;
}

void QTDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPointF lp = event->localPos();
        const double xp = lp.x()/size().width();
        const double yp = 1.0 - lp.y()/size().height();
        const double xs = _drawerInfo.GetXSize();
        const double ys = _drawerInfo.GetYSize();
        const double xCenter = _drawerInfo.xbegin + xp * xs;
        const double yCenter = _drawerInfo.ybegin + yp * ys;

        const DLimits limits
        {
            std::max(_drawerInfo.xbegin, xCenter - xs/4),
            std::max(_drawerInfo.ybegin, yCenter - ys/4),
            std::min(_drawerInfo.xend  , xCenter + xs/4),
            std::min(_drawerInfo.yend  , yCenter + ys/4)
        };
        Zoom(limits);
    }
    else if(event->button() == Qt::RightButton)
    {
        ResetLimits();
    }
}

void QTDrawer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    for(auto drawable: _graphs)
        drawable->Draw(this, &painter);
    for(auto drawable: _overDrawables)
        drawable->Draw(this, &painter);
}


void QTDrawer::ResetLimits()
{
    DLimits limits = DLimits::Invalid();
    for(std::shared_ptr<IGraph> graph: _graphs)
    {
        const DLimits & gLimits = graph->GetDataInfo().limits;
        limits.xbegin = std::min(limits.xbegin, gLimits.xbegin);
        limits.ybegin = std::min(limits.ybegin, gLimits.ybegin);
        limits.xend   = std::max(limits.xend  , gLimits.xend  );
        limits.yend   = std::max(limits.yend  , gLimits.yend  );
    }
    Zoom(limits);
}
