#ifndef INTERFACES_H
#define INTERFACES_H

#include <memory>
#include <vector>
#include "QPainter"
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
        limits.xend   = std::numeric_limits<double>::min();
        limits.yend   = std::numeric_limits<double>::min();
        return limits;
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
    //TODO: iterator GetData(const double xbegin, const double xend) const = 0;
    virtual timevalue GetData() const = 0;
    virtual DataInfo GetDataInfo() const = 0;
};
typedef std::shared_ptr<IDataSource> IDataSourcePtr;

class IDrawer;

class IDrawable
{
public:
    virtual void Draw(IDrawer* drawer, QPainter* painter) = 0;
};

class IGraph: public IDrawable
{
public:
    virtual DataInfo GetDataInfo() = 0;
    //virtual void GetLegendSample(QRect rect) = 0;
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
    virtual void Zoom(const DLimits & limits) = 0;
    virtual DLimits GetDrawingLimits() const = 0;
    virtual void AddDrawable(std::shared_ptr<IGraph> drawable) = 0;
    virtual void AddDrawable(std::shared_ptr<IAxes> drawable) = 0;
    virtual void AddDrawable(std::shared_ptr<ILegend> drawable) = 0;
    virtual std::vector<std::shared_ptr<IGraph>> GetGraphs() = 0;
};
#endif //DLimitsS_H
