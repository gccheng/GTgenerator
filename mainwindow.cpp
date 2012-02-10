#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "opencvheader.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gtv = NULL;
    videoloader = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTimer(int interval, char method)
{
    timer = new QTimer(this);
    if (NULL != timer)
    {
        switch(method)
        {
        case 'S':
            connect(timer, SIGNAL(timeout()), this, SLOT(updateSlider())); break;
        default:
            qDebug() << "MainWindow::setupTimer: Wrong timer out handler.";
            return;
        }
        timer->start(interval);
        qDebug() << QString("Timer ID=%1 started.").arg(timer->timerId());
    }
}

void MainWindow::stopTimer()
{
    qDebug() << QString("Timer ID=%1 stopped.").arg(timer->timerId());
    timer->stop();
}

// Load video from a video file
void MainWindow::on_actionVideo_triggered()
{
    QString filter = tr("Video fiels(*.avi *.mpg  *.webm*.ogg *.ogv );; All files(*.*)");
    QString filePath = QFileDialog::getOpenFileName(this, tr("Import Video"), "", filter);

    // create a thread to load video file
    if ("" != filePath)
    {
        // create GTVideo following delete the previously saved one
        if (NULL != gtv)
        {
            delete gtv;
        }
        gtv = new GTVideo(VIDEO, filePath);

        if (QDir().exists("./source") || QDir().mkdir("./source"))
        {
            if (NULL != videoloader)
            {
                delete videoloader;
            }
            videoloader = new LoadVideoThread(filePath, VIDEO, gtv);
            connect(videoloader, SIGNAL(completeLoading(bool)), this, SLOT(videoload_completed(bool)), Qt::QueuedConnection);
            videoloader->start();
            timer = new QTimer(this);     // timer begins
            connect(timer, SIGNAL(timeout()), this, SLOT(updateSlider()));

            setupTimer(500, 'S');      // timer begins
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot create folder ./source in " + QDir::currentPath());
            msgBox.exec();
        }
    }
}

// Load video from images in a user-specified folder
void MainWindow::on_actionImages_triggered()
{
    // select directory when the frames are
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Import Video"),
                                                        QDir::currentPath(),
                                                        QFileDialog::ShowDirsOnly);
    if ("" != dirPath)
    {
        // create GTVideo following delete the previously saved one
        if (NULL != gtv)
        {
            delete gtv;
        }
        gtv = new GTVideo(IMAGES, dirPath);

        if (QDir().exists("./source") || QDir().mkdir("./source"))
        {
            if (NULL != videoloader)
            {
                delete videoloader;
            }
            videoloader = new LoadVideoThread(dirPath, IMAGES, gtv);
            connect(videoloader, SIGNAL(completeLoading(bool)), this, SLOT(videoload_completed(bool)), Qt::QueuedConnection);
            videoloader->start();

            setupTimer(500, 'S');     // timer begins
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot create folder ./source in " + QDir::currentPath());
            msgBox.exec();
        }
    }
}

void MainWindow::updateSlider()
{
    if (timer->isActive())
    {
        ui->Slider_videoloaded->setMaximum(gtv->getFrameNumber());
        ui->Slider_videoloaded->setSliderPosition(20);
        ui->Slider_videoloaded->update();

        ui->loadvideo_progressBar->setMaximum(gtv->getFrameCount());
        ui->loadvideo_progressBar->setValue(gtv->getFrameNumber());
        ui->loadvideo_progressBar->update();
    }
}

// processing when videoloader thread terminates
void MainWindow::videoload_completed(bool result)
{
    stopTimer();     // timer ends
    if (!result)
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot load video");
        msgBox.exec();
    }
    else
    {
        gtv->setFrameCount(gtv->getFrameNumber());

        ui->Slider_videoloaded->setMaximum(gtv->getFrameCount());
        ui->Slider_videoloaded->setSliderPosition(0);

        // since framecount is inacurate, so update it to 100% when loading finishes
        ui->loadvideo_progressBar->setMaximum(gtv->getFrameCount());
        ui->loadvideo_progressBar->setValue(gtv->getFrameNumber());
        ui->loadvideo_progressBar->update();

        qDebug() << QString("Frames loaded: %1").arg(gtv->getFrameCount());
        qDebug() << "Video/imges have been loaded!\n";
    }
}

void MainWindow::on_Slider_videoloaded_sliderMoved(int position)
{
    qDebug() << "slider is moved!\n";
}

void MainWindow::on_Slider_videoloaded_sliderPressed()
{
    qDebug() << "slider is pressed!\n";
}

void MainWindow::on_Slider_videoloaded_valueChanged(int value)
{
    qDebug() << QString("value is changed to: %1").arg(value);

    //qDebug() << QString("number of frames loaded: %1").arg(gtv->getFrameNumber());

    if (NULL != gtv)
    {
        if(gtv->getFrameNumber()>0)
        {
            //ui->Slider_videoloaded->setMaximum(gtv->getFrameNumber());
            cv::Mat frame_curr=gtv->retrieveFrame(value);
            QImage img_curr= QImage((const unsigned char*)( frame_curr.data),
                                    frame_curr.cols,frame_curr.rows,QImage::Format_RGB888);
            ui->label_fcurr->setPixmap(QPixmap::fromImage(img_curr));
            ui->label_fcurr->setScaledContents(true);

            if(value>1)
            {
                cv::Mat frame_prev=gtv->retrieveFrame(value-1);
                QImage img_prev= QImage((const unsigned char*)( frame_prev.data),
                                        frame_prev.cols,frame_prev.rows,QImage::Format_RGB888);
                ui->label_fprev->setPixmap(QPixmap::fromImage(img_prev));
                ui->label_fprev->setScaledContents(true);


            }
            if(value<ui->Slider_videoloaded->maximum())
            {
                cv::Mat frame_next=gtv->retrieveFrame(value+1);
                QImage img_next= QImage((const unsigned char*)( frame_next.data),
                                        frame_next.cols,frame_next.rows,QImage::Format_RGB888);
                ui->label_fnext->setPixmap(QPixmap::fromImage(img_next));
                ui->label_fnext->setScaledContents(true);
            }

        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Please select video to load!");
        msgBox.exec();
    }


}

void MainWindow::on_Slider_videoloaded_sliderReleased()
{

}

void MainWindow::on_actionAddBoundary_triggered()
{

}
