#include "abnormalrange.h"

AbnormalRange::AbnormalRange(QObject *parent) :
    QObject(parent), start(0), end(0)
{
}

AbnormalRange::AbnormalRange(uint s, uint e, const cv::Mat& r, QObject *parent)
    :QObject(parent), start(s), end(e), roi(r)
{
}

AbnormalRange::AbnormalRange(const AbnormalRange &ar)
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
}

void AbnormalRange::setStartEnd(uint s, uint e)
{
    start = s;
    end = e;
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


