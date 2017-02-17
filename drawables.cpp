#include "drawables.h"

Graph::Graph(IDataSourcePtr dataSource)
    : _dataSource(dataSource)
{
    _linepen.setColor(Qt::black);
    _linepen.setWidthF(0.5f);
    _dotpen.setColor(Qt::green);
    _dotpen.setWidthF(2.0f);
}

void Graph::Draw(IDrawer *drawer, QPainter *painter)
{

    const timevalue & data = _dataSource->GetData();
    {

        int w = painter->device()->width();
        int h = painter->device()->height();

        DLimits limits = drawer->GetDrawingLimits();
        const double visibleDX = limits.GetXSize();
        const double visibleDY = limits.GetYSize();
        DataInfo info = _dataSource->GetDataInfo();
        size_t iXStart = 0;
        //TODO: data is sorted. Use it
        _projected.clear();
        for(size_t i = 1; i<info.nPoints; ++i)
        {
            if(data[i].first >= limits.xbegin)
            {
                iXStart = i-1;
                break;
            }
        }
        for(size_t i = iXStart; i<info.nPoints; ++i)
        {
            QPoint p((data[i].first - limits.xbegin)/visibleDX * w,
                      h - (data[i].second - limits.ybegin)/visibleDY * h);
            _projected.push_back(p);
            if(data[i].first > limits.xend)
                break;
        }
    }
    //TODO: use QPainter transformations
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(_linepen);
    painter->drawPolyline(_projected.data(), _projected.size());
    painter->setPen(_dotpen);
    painter->drawPoints(_projected.data(), _projected.size());
}

void Axes::Draw(IDrawer *drawer, QPainter *painter)
{
    const int w = painter->device()->width();
    const int h = painter->device()->height();

    const DLimits & limits = drawer->GetDrawingLimits();

    //y
    {
        const QPoint axisS(_relativePos.x()*w, h);
        const QPoint axisE(_relativePos.x()*w, 0);
        painter->setPen(_axesPen);
        painter->drawLine(axisS, axisE);
        painter->drawLine(axisE.x(), axisE.y(), axisE.x() - _arrowDelta, axisE.y() + _arrowLen);
        painter->drawLine(axisE.x(), axisE.y(), axisE.x() + _arrowDelta, axisE.y() + _arrowLen);
        for(int iy = 0; iy<h; iy+=_marksShiftY)
        {
            double z = 1.0 - double(iy)/double(h);
            double xx = limits.ybegin + z*limits.GetYSize();
            painter->setPen(_axesPen);
            painter->drawLine(axisS.x() - _arrowDelta, iy, axisS.x() + _arrowDelta, iy);
            painter->setPen(_textPen);
            painter->drawText(axisS.x(), iy, QString::number(xx, 'e', 6));
        }
    }
    //x
    {
        const QPoint axisS(0, _relativePos.y()*h);
        const QPoint axisE(w, _relativePos.y()*h);
        painter->setPen(_axesPen);
        painter->drawLine(axisS, axisE);
        painter->drawLine(axisE.x(), axisE.y(), axisE.x() - _arrowLen , axisE.y() + _arrowDelta);
        painter->drawLine(axisE.x(), axisE.y(), axisE.x() - _arrowLen , axisE.y() - _arrowDelta);
        for(int ix = 0; ix<w; ix+=_marksShiftX)
        {
            double z = double(ix)/double(w);
            double xx = limits.xbegin + z*limits.GetXSize();
            painter->setPen(_axesPen);
            painter->drawLine(ix, axisS.y() + _arrowDelta, ix, axisS.y() - _arrowDelta);
            painter->setPen(_textPen);
            painter->drawText(ix, axisS.y(), QString::number(xx, 'e', 3));
        }

    }
}

void Axes::SetPos(QPointF relativePos)
{
    _relativePos = relativePos;
}

void Legend::Draw(IDrawer *drawer, QPainter *painter)
{
    QRect rect;
    const int w = painter->device()->width();
    const int h = painter->device()->height();
    std::vector<std::shared_ptr<IGraph>> graphs = drawer->GetGraphs();
    rect.setX(_relativePos.x()*w);
    rect.setY(_relativePos.y()*h);
    rect.setWidth(_relativePos.width()*w);
    rect.setHeight(_relativePos.height()*h);
    painter->setPen(QPen(Qt::black));
    painter->drawRect(rect);

    painter->setPen(QPen(Qt::red));
    rect.setHeight(rect.height()/graphs.size());
    for(std::shared_ptr<IGraph> graph: graphs)
    {
        //TODO: graph->GetLegendSample();
        painter->drawText(rect, Qt::AlignLeft, graph->GetDataInfo().header.c_str());
        rect.setY(rect.y() + rect.width());
    }
}

void Legend::SetRect(QRectF relativePos)
{
    _relativePos = relativePos;
}
