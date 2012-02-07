#include "abnormalrange.h"

AbnormalRange::AbnormalRange(QObject *parent) :
    QObject(parent)
{
    start  = 0;
    end    = 0;
    roi    = cv::Mat(); 
}
