#ifndef ABNORMALRANGE_H
#define ABNORMALRANGE_H

#include <QObject>

#include "opencvheader.h"

class AbnormalRange : public QObject
{
    Q_OBJECT
public:
    explicit AbnormalRange(QObject *parent = 0);

signals:

public slots:

private:
    uint start;     // start frame # with anomaly
    uint end;       // end frame # with anomaly
    cv::Mat roi;    // 0-1 matrix showing region of interest (anomaly)
};

#endif // ABNORMALRANGE_H
