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

    void Draw(IDrawer* drawer, QPainter * painter) override;
    DataInfo GetDataInfo() override
    {
        return _dataSource->GetDataInfo();
    }
private:
    IDataSourcePtr _dataSource;
    QPen _linepen;
    QPen _dotpen;
    std::vector<QPoint> _projected;
    DLimits _prevLimits;
};

class Legend: public ILegend
{
public:
    void Draw(IDrawer* drawer, QPainter * painter) override;
    void SetRect(QRectF relativePos) override;
private:
    QRectF _relativePos = QRectF(0.69, 0.01, 0.30, 0.20);
};

class Axes: public IAxes
{
public:
    void Draw(IDrawer* drawer, QPainter * painter) override;
    void SetPos(QPointF relativePos) override;
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

