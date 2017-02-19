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
    //TODO: more precise redraw logic
    //if(!(_drawerInfo == limits))
    {
        _drawerInfo = limits;
        _needRepaint = true;
        update();
    }
}

DLimits QTDrawer::GetDrawingLimits()
{
    return _drawerInfo;
}

std::vector<std::shared_ptr<IGraph> > QTDrawer::GetGraphs()
{
    return _graphs;
}

std::shared_ptr<QPainter> QTDrawer::GetPainter()
{
    return std::make_shared<QPainter>(&_image);
}

bool QTDrawer::IsNeedRedraw()
{
    return _needRepaint;
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

void QTDrawer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    _needRepaint = true;
}

void QTDrawer::paint()
{
    if(IsNeedRedraw())
    {
        _image = QPixmap(this->size());
        _image.fill(QColor(0, 0, 0, 0));
    }
    //TODO: fix drawing order. May be individual layer for each object?
    for(auto drawable: _graphs)
        if(!drawable->Draw(this))
            update();
    for(auto drawable: _overDrawables)
        if(!drawable->Draw(this))
            update();
}

void QTDrawer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    paint();
    _needRepaint = false;
    painter.drawPixmap(this->rect(), _image);
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
