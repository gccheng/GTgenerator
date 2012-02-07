#include "loadvideothread.h"

#include <QTimer>
#include <QDebug>

#include "opencvheader.h"

LoadVideoThread::LoadVideoThread(const QString &path, QThread *parent) :
    filePath(path), QThread(parent)
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
    qDebug() << "Start loading video...";
    cv::VideoCapture cap;
    if(!cap.open(filePath.toStdString()))
    {
        emit completeLoading(false);
        return;
    }
    else
    {
        cv::Mat frame; uint frameno = 0;
        for(;;)
        {
            cap >> frame;
            if (!frame.data)
            {
                break;
            }
            frameno++;
            QString filename = "";
            filename.sprintf("./source/%03d.tif", frameno);
            bool retWrite = cv::imwrite(filename.toStdString(), frame);
            if (!retWrite)
            {
                emit completeLoading(false);
                break;
            }
        }
    }



    emit completeLoading(true);
}
