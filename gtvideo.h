#ifndef GTVIDEO_H
#define GTVIDEO_H

#include <QObject>
#include <QVector>
#include "opencvheader.h"

#include "abnormalrange.h"

enum SourceType {VIDEO = 1, IMAGES = 2};

class GTVideo : public QObject
{
    Q_OBJECT
public:
    explicit GTVideo(QObject *parent = 0);
    explicit GTVideo(SourceType t, QString path, QObject *parent = 0);

public:
    void setSource(const QVector<cv::Mat> &s);
    void appendFrame(const cv::Mat &f);

    int getFrameNumber() const;
    const cv::Mat& retrieveFrame(int pos) const;
    const QVector<cv::Mat>& retrieveFrames() const;

private:
    SourceType type;               // data source type
    QString filePath;              // source file path

    QVector<cv::Mat> source;   // frames of the input video
    QVector<cv::Mat> objects;  // the generated groundtruth
    QVector<AbnormalRange> abnormallist;
};

#endif // GTVIDEO_H
