#ifndef GTVIDEO_H
#define GTVIDEO_H

/*
 * Description: Class GTVideo is a top class for groundtruth
 *              generation. It accepts user's input video as
 *              source, user specified abnormal information,
 *              and generates the groundtruth in objects.
 * Create Date: 02/09/2012
 * Last Update: ##/##/####: ************
 *
 */

#include <QObject>
#include <QVector>
#include "opencvheader.h"

#include "abnormalrange.h"
#include "libseg.h"
#include "sharedtypes.h"

class GTVideo : public QObject
{
    Q_OBJECT
public:
    explicit GTVideo(QObject *parent = 0);
    explicit GTVideo(SourceType t, QString path, QObject *parent = 0);

public:
    void setFrameCount(int fc);              // call with caution, only used during the loading of video
    void setSource(const QVector<cv::Mat> &s);
    void appendFrame(const cv::Mat &f);
    void addAbnormalRange(const AbnormalRange &ar);
    void updateAbnormalRange(const AbnormalRange &ar);
    void addGroundtruth(const cv::Mat &truth, int position = -1);
    void setGroundtruth(const cv::Mat &truth, int position);
    void initializeGroundtruth(int size, const cv::Mat& truth);

    int getFrameNumber() const;
    int getFrameCount() const;
    int getAbnormallistsize() const;

    const cv::Mat& retrieveFrame(int pos) const;
    const QVector<cv::Mat>& retrieveFrames() const;
    const cv::Mat& retrieveGroundtruth(int pos) const;
    const QVector<cv::Mat>& retrieveGroundtruth() const;

public:
    void generateGroundtruth(TrackType tracktype);      // generate groundtruth through tracking
    void setBackground();
    void setForegroundMask();                           // set foreground for all the video frames
    void estimateBackground();
    void subtractBackground(cv::Mat& foreground_mask,const cv::Mat& frame_cur); // subtract background image from source to get foreground mask
    uchar quick_select(uchar* arr, int n);              // quicksort for median value of the arr
    bool saveSourceToFiles(QString strPath);
    bool saveGroundtruthToFiles(QString strPath);
    void postProcessing(int strelsize);                 // post-processing the groundtruth images using morphological openning

private:
    void snakeTracking();                               // tracking using snake image
    void snakeTracking2();                              // a better implementation of snake tracking
    cv::Mat segmentByActiveContour(const cv::Mat& aSrc, const cv::Mat& aInitMask, int aMaxIts, bool aDisp);
    int getAbnormalFrameCount() const;

private:
    QVector<cv::Mat> source;                // frames of the input video
    QVector<cv::Mat> grdtruth;              // the generated groundtruth
    QVector<cv::Mat> foregroundMask;        // foreground
    QVector<AbnormalRange> abnormallist;    // where anomaly starts/ends

    cv::Mat background; //background image in RGB

    SourceType type;                // data source type
    QString filePath;               // source file path
    int frameCount;                 // total number of frames in the video (maybe VERY inaccurate before
                                    // the video is completely loaded
    TrackType trackType;            // method for tracking
};

#endif // GTVIDEO_H
