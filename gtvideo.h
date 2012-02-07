#ifndef GTVIDEO_H
#define GTVIDEO_H

#include <QObject>
#include <vector>
#include <opencv2/core/core.hpp>

#include "abnormalrange.h"

class GTVideo : public QObject
{
    enum SourceType {video = 1, images = 2};

    Q_OBJECT
public:
    explicit GTVideo(QObject *parent = 0);

public:


private:
    SourceType type;               // data source type
    QString filePath;              // source file path

    std::vector<cv::Mat> source;   // frames of the input video
    std::vector<cv::Mat> objects;  // the generated groundtruth
    std::vector<AbnormalRange> abnormallist;

};

#endif // GTVIDEO_H
