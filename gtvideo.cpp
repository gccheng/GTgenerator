#include "gtvideo.h"
#include <QDebug>

#include <opencv2/legacy/legacy.hpp>

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

void GTVideo::updateAbnormalRange(const AbnormalRange &ar)
{
    abnormallist.pop_back();
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

void GTVideo::setGroundtruth(const cv::Mat &truth, int position)
{
    if ((position>=grdtruth.size()) || (position<0))
    {
        return;
    }
    else
    {
        grdtruth[position] = truth;
    }

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

 // generator the abnormal results through tracking
 void GTVideo::generateGroundtruth(TrackType tracktype)
 {
     switch (tracktype)
     {
     case SNAKE:
         snakeTracking();
     }
 }

 void GTVideo::snakeTracking()
 {
     if(source.isEmpty() || abnormallist.isEmpty())
     {
         qDebug() << "Video source and initial abnormal range must be set before tracking\n";
         return;
     }

     //initialize the groundtruth
     cv::Mat eye = source.at(0);
     eye.setTo(cv::Scalar(0)); //cv::Scalar(0,0,0)
     grdtruth.fill(eye);

     for (int iAb=0; iAb<abnormallist.size(); iAb++)
     {
         uint start = abnormallist[iAb].getStart();
         uint end = abnormallist[iAb].getEnd();
         int length = end-start+1;
         const QVector<cv::Point>& boundaryPoints = abnormallist[iAb].getBoundaryPoints();

         // consctruct a new array of type CvPoint because it will be modified for each frame
         const int npts = boundaryPoints.size();
         CvPoint pts_snake[npts];
         for (int i=0; i<npts; i++)
         {
             pts_snake[i] = boundaryPoints[i];
         }

         // set parameters for cvSnakeImage()
         float alpha = 0.2f;
         float beta = 0.2f;
         float gamma = 0.2f;
         int coeff_usage = CV_VALUE;
         CvSize win = cvSize(5,5);
         CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.1);

         // set tracked object as abnormal ROI
         for (uint iFrame=start; iFrame<=end; iFrame++)
         {
             // update boundary using that in previous frame
             cv::Mat grayFrame;
             cv::cvtColor(source[iFrame], grayFrame, CV_RGB2GRAY);
             IplImage *ipFrame = new IplImage(grayFrame);
             cvSnakeImage(ipFrame, pts_snake, npts, &alpha, &beta, &gamma, coeff_usage, win, criteria, 1);

             cvSaveImage("frame.tif", ipFrame);

             // convert boundary points from CvPoint[] to vector<Point>
             std::vector<cv::Point> stdBoundPoints;
             for (int i=0; i<npts; i++)
             {
                 cv::Point p(pts_snake[i].x, pts_snake[i].y);
                 stdBoundPoints.push_back(p);
             }

             // fill the empty grayFrame using popygon to get roi
             cv::Mat roi;
             cv::cvtColor(source[iFrame], roi, CV_RGB2GRAY);
             roi.setTo(cv::Scalar(0)); //cv::Scalar(0,0,0)
             //cv::fillPoly(roi, stdBoundPoints, cv::Scalar(0)); //cv::Scalar(255,255,255)
             const cv::Point *pAddBoundary = stdBoundPoints.data();
             const cv::Point **pBoundaryPoints = &pAddBoundary;
             cv::fillPoly(roi, pBoundaryPoints, &npts, 1, cv::Scalar(255));  //cv::Scalar(255,255,255)

             abnormallist[iAb].setROI(roi);
             setGroundtruth(roi, iFrame);

             delete ipFrame;

             cv::imwrite("output.tif", roi);
         }
     }
 }
