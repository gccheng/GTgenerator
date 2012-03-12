#include "gtvideo.h"
#include <QDebug>
#include <QtCore/qmath.h>

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
     if(foregroundMask.isEmpty() || abnormallist.isEmpty())
     {
         qDebug() << "Video source and initial abnormal range must be set before tracking\n";
         return;
     }

     //initialize the groundtruth
     cv::Mat eye = foregroundMask.at(0);
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
         float alpha = 0.5f;
         float beta = 0.5f;
         float gamma = 0.5f;
         int coeff_usage = CV_VALUE;
         CvSize win = cvSize(21,21);
         CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.5);

         // set tracked object as abnormal ROI
         for (uint iFrame=start; iFrame<=end; iFrame++)
         {
             // update boundary using that in previous frame
             cv::Mat grayFrame(foregroundMask[iFrame]);
             //cv::cvtColor(foregroundMask[iFrame], grayFrame, CV_RGB2GRAY);
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
             cv::Mat roi(foregroundMask[iFrame]);
             //cv::cvtColor(foregroundMask[iFrame], roi, CV_RGB2GRAY);
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
 void GTVideo::setBackground()
 {
     background = source.at(0).clone();
 }
 void GTVideo::subtractBackground(cv::Mat& foreground_mask,const cv::Mat& frame_cur)
 {
  int thr_diff = 30;
  uchar dR;
  uchar dG;
  uchar dB;

  int width = source.at(0).cols;
  int height = source.at(0).rows;

  for(int i=0;i<height;i++)
      for(int j=0;j<width;j++)
      {
          dR = abs(uchar(frame_cur.data[(i*width+j)*3])-background.data[(i*width+j)*3]);
          dG = abs(uchar(frame_cur.data[(i*width+j)*3 + 1])-background.data[(i*width+j)*3 + 1]);
          dB = abs(uchar(frame_cur.data[(i*width+j)*3 + 2])-background.data[(i*width+j)*3 + 2]);

          if(qMax(qMax(dB,dG),dR)>thr_diff)
          {
              foreground_mask.data[i*width+j] = 255;
          }
          else
          {
              foreground_mask.data[i*width+j] = 0;
          }

      }
  //to smooth out noise
  //cv::erode(foreground_mask,foreground_mask,cv::Mat(cv::Size(3, 3),CV_8UC1));
  //cv::dilate(foreground_mask,foreground_mask,cv::Mat(cv::Size(3, 3),CV_8UC1));




 }
 void GTVideo::setForegroundMask()
 {


     for (int i=0; i<getFrameNumber(); i++)
     {
         cv::Mat foreground_mask(source.at(0).rows,source.at(0).cols,CV_8UC1);
         subtractBackground(foreground_mask,source.at(i));
         foregroundMask.push_back(foreground_mask);
     }

     //cv::imwrite("frame_cur.jpg",source.at(55));
     //cv::imwrite("foregroundmask.jpg",foregroundMask.at(55));
 }

 void GTVideo::estimateBackground()
 {
  int buffer_size = 30;

  uchar* arr_R = new uchar[buffer_size];
  uchar* arr_G = new uchar[buffer_size];
  uchar* arr_B = new uchar[buffer_size];

  int width = source.at(0).cols;
  int height = source.at(0).rows;

  for(int i=0;i<height;i++)
      for(int j=0;j<width;j++)
         {
          for(int k=1;k<=buffer_size;k++)
             {
              arr_R[k]= source.at(k*10).data[(i*width+j)*3];
              arr_G[k]= source.at(k*10).data[(i*width+j)*3 + 1];
              arr_B[k]= source.at(k*10).data[(i*width+j)*3 + 2];
             }
          background.data[(i*width+j)*3]=quick_select(arr_R,buffer_size);
          background.data[(i*width+j)*3+1]=quick_select(arr_G,buffer_size);
          background.data[(i*width+j)*3+2]=quick_select(arr_B,buffer_size);
         }

  cv::imwrite("background.jpg",background);
 }

uchar GTVideo::quick_select(uchar* arr, int n)
{
    int low, high ;
     int median;
     int middle, ll, hh;
     uchar t;
     low = 0 ; high = n-1 ; median = (low + high) / 2;
     for (;;)
     {
         if (high <= low) /* One element only */
             return arr[median] ;

         if (high == low + 1) {  /* Two elements only */
             if (arr[low] > arr[high])
                 t=arr[low];
                 arr[low]=arr[high];
                 arr[high]=t;
             return arr[median] ;}

     /* Find median of low, middle and high items; swap into position low */
     middle = (low + high) / 2;
     if (arr[middle] > arr[high])
     {      t=arr[middle];
            arr[middle]=arr[high];
            arr[high]=t;
     }
     if (arr[low] > arr[high])
     {      t=arr[low];
            arr[low]=arr[high];
            arr[high]=t;
     }
     if (arr[middle] > arr[low])
     {      t=arr[middle];
            arr[middle]=arr[low];
            arr[low]=t;
     }

     /* Swap low item (now in position middle) into position (low+1) */
     {      t=arr[middle];
            arr[middle]=arr[low+1];
            arr[low+1]=t;
     }

     /* Nibble from each end towards middle, swapping items when stuck */
     ll = low + 1;
     hh = high;
     for (;;) {
         do ll++; while (arr[low] > arr[ll]) ;
         do hh--; while (arr[hh]  > arr[low]) ;

         if (hh < ll)
         break;

         {  t=arr[ll];
            arr[ll]=arr[hh];
            arr[hh]=t;
         }

     }

     /* Swap middle item (in position low) back into correct position */
            t=arr[low];
            arr[low]=arr[hh];
            arr[hh]=t;


     /* Re-set active partition */
     if (hh <= median)
         low = ll;
         if (hh >= median)
         high = hh - 1;
     }
}
