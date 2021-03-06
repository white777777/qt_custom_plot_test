#ifndef DRAWABLES_H
#define DRAWABLES_H

#include "interfaces.h"
#include <QPainter>
#include <QRect>
#include <QPen>

class Graph: public IGraph
{
public:
    Graph(IDataSourcePtr dataSource);

    bool Draw(IDrawer* drawer) override;
    DataInfo GetDataInfo() override;
    QImage GetLegendSample(QSize size) override;

    void SetColor(QColor pen);

private:
    IDataSourcePtr _dataSource;
    QPen _linepen;
    QPen _dotpen;
    size_t _iXStart = 0;
    void findDataStart(const DLimits &limits, const timevalue& data, const DataInfo &info);
    bool _frameFinished = false;
};

class Legend: public ILegend
{
public:
    bool Draw(IDrawer* drawer) override;
    void SetRect(QRectF relativePos) override;
private:
    QRectF _relativePos = QRectF(0.69, 0.01, 0.30, 0.20);
};

class Axes: public IAxes
{
public:
    bool Draw(IDrawer* drawer) override;
    void SetPos(QPointF relativePos) override;
protected:
    void drawYAxis(const DLimits& limits, QPainter *painter);
    void drawXAxis(const DLimits& limits, QPainter *painter);
private:
    const int _arrowLen = 3;
    const int _arrowDelta = 2;
    const int _marksShiftY = 50;
    const int _marksShiftX = 100;
    QPen _axesPen = QPen(Qt::black, 1.0f, Qt::SolidLine);
    QPen _textPen = QPen(Qt::red, 1.0f, Qt::SolidLine);
    QPointF _relativePos = QPointF(0.5, 0.5);
};

#endif // DRAWABLES_H

