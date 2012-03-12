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
         snakeTracking2();
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

 void GTVideo::snakeTracking2()
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
         const int npts = boundaryPoints.size();
         const cv::Point *pAddBoundary = boundaryPoints.data();
         const cv::Point **pBoundaryPoints = &pAddBoundary;

         // initialize the segmentation mask
         cv::Mat initmask;
         cv::cvtColor(source[0], initmask, CV_RGB2GRAY);
         initmask.setTo(cv::Scalar(0));
         cv::fillPoly(initmask, pBoundaryPoints, &npts, 1, cv::Scalar(255));

         // set tracked object as abnormal ROI
         for (uint iFrame=start; iFrame<=end; iFrame++)
         {
             // update boundary using that in previous frame
             cv::Mat grayFrame;
             cv::cvtColor(source[iFrame], grayFrame, CV_RGB2GRAY);

             cv::Mat resultImage = segmentByActiveContour(grayFrame, initmask, 250, false);
             initmask = resultImage;

             cv::imwrite("frame.tif", grayFrame);
             cv::imwrite("result.tif", resultImage);
         }
     }
 }

 cv::Mat GTVideo::segmentByActiveContour(const cv::Mat& aSrc, const cv::Mat& aInitMask, int aMaxIts, bool aDisp)
 {
     if (NULL == aSrc.data)
     {
         std::cerr << "Could not load the image."
                   <<  std::endl;
         return cv::Mat();
     }

     // Initialize the MATLAB Compiler Runtime global state
     if (!mclInitializeApplication(NULL,0))
     {
         std::cerr << "Could not initialize the application properly."
                   <<  std::endl;
         return cv::Mat();
     }

     // Initialize the segmentation library
     if (!libsegInitialize())
     {
         std::cerr << "Could not initialize the library properly."
                   << std::endl;
         return cv::Mat();
     }

     const int rows = aSrc.rows;
     const int cols = aSrc.cols;

     // set input parameters to the shared object libseg.so
     // source image (Note: matlab is column-major while C++ is row-major
     cv::Mat trImage;
     cv::transpose(aSrc, trImage);
     mwArray I(rows, cols, mxUINT8_CLASS);
     I.SetData(trImage.data, rows*cols);

     // maximum iteration number
     int maxits[1] = {aMaxIts};
     mwArray max_its(1,1,  mxINT32_CLASS);//mxUINT8_CLASS);
     max_its.SetData(maxits, 1);

     // initial mask of segmentation
     cv::Mat initmask = aInitMask.clone();
     mwArray init_mask(rows, cols, mxUINT8_CLASS);
     init_mask.SetData(initmask.data, rows*cols);

     // parameter alpha to the active contour method
     double alph[1] = {0.2};
     mwArray alpha(1,1, mxDOUBLE_CLASS);
     alpha.SetData(alph, 1);

     // display intermediate results or not
     uchar disp[1] = {aDisp?1:0};
     mwArray display(1,1, mxUINT8_CLASS);
     display.SetData(disp, 1);

     // return value container
     mwArray imOutput(rows, cols, mxUINT8_CLASS);
     region_seg(1, imOutput, I, init_mask, max_its, alpha, display);

     // retrieve result to cv::Mat format
     uchar result[rows*cols];
     imOutput.GetData(result, rows*cols);
     //mxClassID classid = imOutput.ClassID();
     //mwSize nDim = imOutput.NumberOfDimensions();
     //const mwArray dims = imOutput.GetDimensions();
     cv::Mat retImage(rows, cols, CV_8UC1);
     for (int j=0; j<cols; j++)
     {
         for (int i=0; i<rows; i++)
         {
             retImage.at<uchar>(i,j) = result[j*rows+i];
         }
     }

      mclTerminateApplication();
      libsegTerminate();

      return retImage;
 }

 void GTVideo::subtractBackground(const cv::Mat& bkgd)
 {

 }
