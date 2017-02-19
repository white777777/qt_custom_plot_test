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
    std::shared_ptr<QPainter> GetPainter() override;
    bool IsNeedRedraw() override;

    void mouseReleaseEvent ( QMouseEvent * event ) override;
    void resizeEvent(QResizeEvent* event) override;

    void AddDrawable(std::shared_ptr<IGraph> drawable);
    void AddDrawable(std::shared_ptr<IAxes> drawable);
    void AddDrawable(std::shared_ptr<ILegend> drawable);
    void Zoom(const DLimits & limits);
    void ResetLimits();
    void Clear()
    {
        _graphs.clear();
        ResetLimits();
    }
    QImage GetImage()
    {
        return _image.toImage();
    }
    void RemoveLastGraph()
    {
        _graphs.pop_back();
        ResetLimits();
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void paint();

signals:

public slots:

private:
    std::vector<std::shared_ptr<IGraph>> _graphs;
    std::vector<std::shared_ptr<IDrawable>> _overDrawables;
    DLimits _drawerInfo = DLimits::Invalid();
    QPixmap _image;
    bool _needRepaint = true;
};

#endif // QTDRAWER_H
