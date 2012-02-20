#include "abnormalrange.h"

AbnormalRange::AbnormalRange(QObject *parent) :
    QObject(parent), start(0), end(0)
{
}

AbnormalRange::AbnormalRange(uint s, uint e, const cv::Mat& r, QObject *parent)
    :QObject(parent), start(s), end(e), roi(r)
{
}

AbnormalRange::AbnormalRange(const AbnormalRange &ar):QObject()
{
    start = ar.start;
    end = ar.end;
    roi = ar.roi;
}

AbnormalRange& AbnormalRange::operator=(const AbnormalRange & ar)
{
    start = ar.start;
    end = ar.end;
    roi = ar.roi;
    return (*this);
}

void AbnormalRange::setStartEnd(uint s, uint e)
{
    start = s;
    end = e;
}

void AbnormalRange::setStart(uint s)
{
    start = s;
}

void AbnormalRange::setEnd(uint e)
{
    end = e;
}

void AbnormalRange::setBoundaryPoints(const QVector<cv::Point> &bdrypoints)
{
    boundarypoints = bdrypoints;
}

void AbnormalRange::setBoundaryPoints(const QVector<QPoint> &bdrypoints)
{
    QVector<QPoint>::const_iterator it = bdrypoints.begin();
    while(it != bdrypoints.end())
    {
        cv::Point point(it->x(), it->y());
        boundarypoints.push_back(point);
    }
}

void AbnormalRange::setROI(const cv::Mat& r)
{
    roi = r;
}


uint AbnormalRange::getStart() const
{
    return start;
}

uint AbnormalRange::getEnd() const
{
    return end;
}

// a new copy of boundary points
const QVector<cv::Point>& AbnormalRange::getBoundaryPoints() const
{
    return boundarypoints;
}

