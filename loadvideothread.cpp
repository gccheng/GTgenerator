#include "loadvideothread.h"

#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QStringList>

#include "opencvheader.h"

LoadVideoThread::LoadVideoThread(const QString &path, SourceType t, GTVideo *v, QThread *parent) :
    QThread(parent), type(t), filePath(path), gtv(v)
{
    moveToThread(this);
}

void LoadVideoThread::run()
{
    QTimer::singleShot(0, this, SLOT(startLoading()));

    exec();
}


void LoadVideoThread::startLoading()
{
    if (type == IMAGES)
    {
        loadFromImages();
    }
    else
    {
        loadFromVideo();
    }
}

void LoadVideoThread::loadFromVideo()
{
    qDebug() << "Start loading video...";

    cv::VideoCapture cap;
    if(!cap.open(filePath.toStdString()))
    {
        emit completeLoading(false);
        return;
    }
    else
    {
        gtv->setFrameCount((int)cap.get(CV_CAP_PROP_FRAME_COUNT));
        uint frameno = 0;
        for(;;)
        {
            cv::Mat frame;
            cap >> frame;
            if (!frame.data)
            {
                break;
            }
            frameno++;
            cv::Mat rgbFrame;
            cv::cvtColor(frame, rgbFrame, CV_BGR2RGB);

            // save image to file
            QString filename = "";
            filename.sprintf("./source/%03d.tif", frameno);
            bool retWrite = cv::imwrite(filename.toStdString(), rgbFrame);
            if (!retWrite)
            {
                emit completeLoading(false);
                break;
            }
            // append it to data model
            gtv->appendFrame(rgbFrame);
        }
        cv::Mat truth((int)cap.get(CV_CAP_PROP_FRAME_HEIGHT),
                      (int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
                      CV_8UC1, cv::Scalar(0));
        gtv->initializeGroundtruth(frameno, truth);
        emit completeLoading(true);
        exit(0);
    }
}

void LoadVideoThread::loadFromImages()
{
    qDebug() << "Start loading frames...";

    QDir dirImages(filePath);
    QFileInfoList listImages = dirImages.entryInfoList(QStringList() << "*.tif" << "*.tiff" << "*.jpg" << "*.png");

    if (0 == listImages.count())
    {
        emit completeLoading(false);
    }
    {
        int frameno = 0;
        QFileInfoList::const_iterator it = listImages.begin();
        while(it != listImages.end())
        {
            cv::Mat frame = cv::imread(it->filePath().toStdString());
            cv::Mat rgbFrame;
            cv::cvtColor(frame, rgbFrame, CV_BGR2RGB);

            frameno++;  QString filename = "";
            filename.sprintf("./source/%03d.tif", frameno);
            bool retWrite = cv::imwrite(filename.toStdString(), rgbFrame);
            if (!retWrite)
            {
                emit completeLoading(false);
                break;
            }

            gtv->appendFrame(rgbFrame);
            it++;
        }
        emit completeLoading(true);
    }
    exit(0);
}
