
#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "opencvheader.h"
#include "window_addabnormalrange.h"


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

GTVideo* MainWindow::getGTVideo() const
{
    return gtv;
}

//to open window to addabnormalrange with initial setup
void MainWindow::open_window_addabnormalrange()
{
    newwindow_addAbnormalRange = new window_addAbnormalRange();
    newwindow_addAbnormalRange->setMainWindow(this);
    newwindow_addAbnormalRange->setGTVideo(gtv);
    newwindow_addAbnormalRange->show();
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
        //qDebug() << QString("Timer ID=%1 started.").arg(timer->timerId());
    }
}

void MainWindow::stopTimer()
{
    //qDebug() << QString("Timer ID=%1 stopped.").arg(timer->timerId());
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
        // since framecount is inacurate, so update it to 100% when loading finishes

        ui->loadvideo_progressBar->setMaximum(gtv->getFrameCount()-1);
        ui->loadvideo_progressBar->setValue(gtv->getFrameNumber()-2);
        ui->loadvideo_progressBar->update();

        qDebug() << QString("Frames loaded: %1").arg(gtv->getFrameCount());

        //to generate background image of the loaded video
        gtv->setBackground();
        gtv->estimateBackground();

        //to substract backgroud to get foregroundmask for every frame
        gtv->setForegroundMask();
    }
}

void MainWindow::on_Slider_videoloaded_sliderMoved(int position)
{

}

void MainWindow::on_Slider_videoloaded_sliderPressed()
{
    qDebug() << "slider is pressed!\n";

}

void MainWindow::on_Slider_videoloaded_valueChanged(int value)
{

    qDebug() << QString("value is changed to: %1").arg(value);

    if (NULL != gtv)
    {
        if(gtv->getFrameNumber()>0 && value>=0 && value<gtv->getFrameCount())
        {
            //ui->Slider_videoloaded->setMaximum(gtv->getFrameNumber());
            cv::Mat frame_curr=gtv->retrieveFrame(value);
            QImage img_curr= QImage((const unsigned char*)( frame_curr.data),
                                    frame_curr.cols,frame_curr.rows,QImage::Format_RGB888);

            ui->label_fcurr->setPixmap(QPixmap::fromImage(img_curr));
            ui->label_fcurr->setScaledContents(true);

            ui->label_fcurr->update();
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
    if (NULL != gtv)
    {
    open_window_addabnormalrange();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Please select video to load!");
        msgBox.exec();
    }
}

void MainWindow::on_Button_prev_clicked()
{


}

void MainWindow::on_actionGroundtruth_2_triggered()
{
    ///--->test
    if (QDir().exists("./result") || QDir().mkdir("./result"))
    {
        gtv->generateGroundtruth(SNAKE);
    }
    ///<---test
}
