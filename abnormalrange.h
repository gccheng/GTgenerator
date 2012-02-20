#ifndef ABNORMALRANGE_H
#define ABNORMALRANGE_H

/*
 * Description: Class AbnormalRange manages the information
 *              specifies through user's interaction, including
 *              the start/end frame # of anomaly and the region
 *              of interest in the start-frame.
 * Create Date:
 * Last Update: ##/##/####: ************
 *
 */

#include <QObject>
#include <QPoint>
#include <QVector>
#include "opencvheader.h"

class AbnormalRange : public QObject
{
    Q_OBJECT
public:
    explicit AbnormalRange(QObject *parent = 0);
    explicit AbnormalRange(uint s, uint e, const cv::Mat& r = cv::Mat(), QObject *parent = 0);
    AbnormalRange(const AbnormalRange &ar);
    AbnormalRange& operator=(const AbnormalRange & ar);

public:
    void setStartEnd(uint s, uint e);
    void setStart(uint s);
    void setEnd(uint e);
    void setBoundaryPoints(const QVector<cv::Point>& bdrypoints);
    void setBoundaryPoints(const QVector<QPoint>& bdrypoints);

    void setROI(const cv::Mat& r);

    uint getStart() const;
    uint getEnd() const;
    const QVector<cv::Point>& getBoundaryPoints() const;

signals:
public slots:

private:
    uint start;                         // start frame # with anomaly
    uint end;                           // end frame # with anomaly
    QVector<cv::Point> boundarypoints;  // user-defined boundary points

    cv::Mat roi;    // 0-1 matrix showing region of interest (anomaly)
};

#endif // ABNORMALRANGE_H
