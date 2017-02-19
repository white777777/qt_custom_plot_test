#ifndef INTERFACES_H
#define INTERFACES_H

#include <memory>
#include <vector>
#include <QPainter>
#include <QImage>

typedef std::vector<std::pair<double, double>> timevalue;

struct DLimits
{
    double xbegin, ybegin, xend, yend;

    double GetXSize() const
    {
        return xend - xbegin;
    }
    double GetYSize() const
    {
        return yend - ybegin;
    }
    static DLimits Invalid()
    {
        DLimits limits;
        limits.xbegin = std::numeric_limits<double>::max();
        limits.ybegin = std::numeric_limits<double>::max();
        limits.xend   = -std::numeric_limits<double>::max();
        limits.yend   = -std::numeric_limits<double>::max();
        return limits;
    }
    bool operator==(const DLimits& rhs)
    {
        return memcmp(this, &rhs, sizeof(DLimits)) == 0;
    }
};

struct DataInfo
{
    std::string header;
    std::string measuerType;
    std::string timeStampStart;
    std::string measureParams;
    DLimits limits;
    size_t nPoints;
};

class IDataSource
{
public:
    //TODO: Add ability to get data with given limits and return iterator to minimize memory usage
    virtual const timevalue& GetData() const = 0;
    virtual DataInfo GetDataInfo() const = 0;
};
typedef std::shared_ptr<IDataSource> IDataSourcePtr;

class IDrawer;

class IDrawable
{
public:
    virtual bool Draw(IDrawer* drawer) = 0;
};

class IGraph: public IDrawable
{
public:
    virtual DataInfo GetDataInfo() = 0;
    virtual QImage GetLegendSample(QSize rect) = 0;
};

class ILegend: public IDrawable
{
public:
    virtual void SetRect(QRectF relativePos) = 0;
};

class IAxes: public IDrawable
{
public:
    virtual void SetPos(QPointF relativePos) = 0;
};

class IDrawer
{
public:
    virtual DLimits GetDrawingLimits() = 0;
    virtual std::vector<std::shared_ptr<IGraph>> GetGraphs() = 0;
    virtual std::shared_ptr<QPainter> GetPainter() = 0;
    virtual bool IsNeedRedraw() = 0; // if size changed || zoomed
};
#endif //DLimitsS_H
