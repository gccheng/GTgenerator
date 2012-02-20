#include "gtvideo.h"
#include <QDebug>

GTVideo::GTVideo(QObject *parent) :
    QObject(parent)
{
}

GTVideo::GTVideo(SourceType t, QString path, QObject *parent)
    :QObject(parent), type(t),filePath(path)
{

}

void GTVideo::setFrameCount(int fc)
{
    frameCount = fc;
}

// the count of frames in the video, equal to getFrameNumber() when the video is completed loaded.
// Before that, the framecount may be not accurate depending on the ecoding format fo the video.
int GTVideo::getFrameCount() const
{
    return frameCount;
}

void GTVideo::setSource(const QVector<cv::Mat> &s)
{
    source = s;
}

void GTVideo::appendFrame(const cv::Mat &f)
{
    source.push_back(f);
}

void GTVideo::addAbnormalRange(const AbnormalRange &ar)
{
    abnormallist.push_back(ar);
}


void GTVideo::addGroundtruth(const cv::Mat &truth, int position)
{
    // If position's not specified, append truth at the end
    // Note that positon is zero-based
    if (-1 == position)
    {
        position = abnormallist.size();
    }

    int n=0;
    QVector<cv::Mat>::iterator it = grdtruth.begin();
    while ((it != grdtruth.end()) && (n<=position))
    {
        it++;
    }
    grdtruth.insert(it, truth);
}

// get the number of currently loaded frames
int GTVideo::getFrameNumber() const
{
    return source.size();
}

int GTVideo::getAbnormallistsize()const
{
    return abnormallist.size();

}

const QVector<cv::Mat>& GTVideo::retrieveFrames() const
{
    return source;
}

 const cv::Mat& GTVideo::retrieveFrame(int pos) const
{
    int sz = source.size();
    if (pos >= sz)
    {
        qDebug() << "GTVideo::retrieveFrame(int): Frame number out of bound (result unreliable).";
        return cv::Mat();
    }
    else
    {
        return source.at(pos);
    }
}

 const cv::Mat& GTVideo::retrieveGroundtruth(int pos) const
 {
     int sz = grdtruth.size();
     if (pos >= sz)
     {
         qDebug() << "GTVideo::retrieveGroundtruth(int): Frame number out of bound (result unreliable).";
         return cv::Mat();
     }
     else
     {
         return source.at(pos);
     }
 }

 const QVector<cv::Mat>& GTVideo::retrieveGroundtruth() const
 {
    return grdtruth;
 }

 void GTVideo::generateGroundtruth()
 {

 }
