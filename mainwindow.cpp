
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "opencvheader.h"
#include "window_addabnormalrange.h"
#include "configuration.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gtv = NULL;
    videoloader = NULL;
    currState = UNINITIALIZED;
}

MainWindow::~MainWindow()
{
    delete ui;
}

GTVideo* MainWindow::getGTVideo() const
{
    return gtv;
}

void MainWindow::setState(States s)
{
    currState = s;
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
    // check if the video is already loaded
    if ((int)currState >= (int)VIDEO_LOADED)
    {
        QMessageBox msgBox;
        msgBox.setText("Video has already been loaded.");
        msgBox.setInformativeText("Do you want to overwrite it?");
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (QMessageBox::No == ret)
        {
            return;
        }
        else
        {
            delete gtv;
            gtv = NULL;
            currState = UNINITIALIZED;
        }
    }

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
}

// Load video from images in a user-specified folder
void MainWindow::on_actionImages_triggered()
{
    // check if the video is already loaded
    if ((int)currState >= (int)VIDEO_LOADED)
    {
        QMessageBox msgBox;
        msgBox.setText("Video has already been loaded.");
        msgBox.setInformativeText("Do you want to overwrite it?");
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        if (QMessageBox::No == ret)
        {
            return;
        }
        else
        {
            delete gtv;
            gtv = NULL;
            currState = UNINITIALIZED;
        }
    }

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

        if (NULL != videoloader)
        {
            delete videoloader;
        }
        videoloader = new LoadVideoThread(dirPath, IMAGES, gtv);
        connect(videoloader, SIGNAL(completeLoading(bool)), this, SLOT(videoload_completed(bool)), Qt::QueuedConnection);
        videoloader->start();
        timer = new QTimer(this);     // timer begins
        connect(timer, SIGNAL(timeout()), this, SLOT(updateSlider()));

        setupTimer(500, 'S');     // timer begins
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
        ui->Slider_videoloaded->setValue(1);
        // since framecount is inacurate, so update it to 100% when loading finishes

        ui->loadvideo_progressBar->setMaximum(gtv->getFrameCount()-1);
        ui->loadvideo_progressBar->setValue(gtv->getFrameNumber()-1);
        ui->loadvideo_progressBar->update();

        qDebug() << QString("Frames loaded: %1").arg(gtv->getFrameCount());

        // to generate background image of the loaded video
        gtv->setBackground();
        gtv->estimateBackground();

        // to substract backgroud to get foregroundmask for every frame
        gtv->setForegroundMask();

        // set state
        currState = VIDEO_LOADED;
    }
}

void MainWindow::on_Slider_videoloaded_valueChanged(int value)
{
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

            QString strCurrPos = "";
            strCurrPos.append(QString("Frame:%1").arg(value));
            statusBar()->showMessage(strCurrPos);

            ui->label_fcurr->update();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Please select video to preview!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

}

void MainWindow::on_actionAddBoundary_triggered()
{
    if (((int)currState>=(int)VIDEO_LOADED) && (NULL != gtv))
    {
        open_window_addabnormalrange();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Please select video to load!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

// previous frame
void MainWindow::on_Button_prev_clicked()
{
    int currFramePos = ui->Slider_videoloaded->value();
    if (currFramePos>=1)
    {
        emit ui->Slider_videoloaded->setSliderPosition(currFramePos-1);
    }
}

// next frame
void MainWindow::on_Button_next_clicked()
{
    int currFramePos = ui->Slider_videoloaded->value();
    if (currFramePos<gtv->getFrameNumber())
    {
        emit ui->Slider_videoloaded->setSliderPosition(currFramePos+1);
    }
}

void MainWindow::on_actionGroundtruth_2_triggered()
{
    if ((int)currState >= (int)CONF_FINISHED)
    {
        // generate groundtruth
        gtv->generateGroundtruth(enumTrackAlgo);

        // if post-processing (morphological openning) is specified
        if (bPostProcessingGT)
        {
            gtv->postProcessing(strelsize);
        }

        // save results
        if (gtv->saveSourceToFiles(strOriginalFrames) &&
            gtv->saveGroundtruthToFiles(strGroundtruthSavePath))
        {
            currState = TRUTH_SAVED;
        }
    }
    else
    {
        QMessageBox msgBox;
        QString warning = "Finish the following prior generating groundtruth:\n"
                "     Loading video;\n"
                "     Selecting abnormal ranges;\n"
                "     Configuration;";
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(warning);
        msgBox.exec();
    }
}

void MainWindow::on_actionConfiguration_triggered()
{
    configuration *pConf = new configuration();

    qRegisterMetaType<ConfigParam>("ConfigParam");
    connect(pConf, SIGNAL(configuration_finished(ConfigParam)),
            this, SLOT(set_configuration(ConfigParam)), Qt::QueuedConnection);

    pConf->open();
}

void MainWindow::set_configuration(QString gtPath, QString origframePath, TrackType trackAlgo)
{
    strGroundtruthSavePath = gtPath;
    strOriginalFrames = origframePath;
    enumTrackAlgo = trackAlgo;

    currState = CONF_FINISHED;
}

void MainWindow::set_configuration(ConfigParam config)
{
    strGroundtruthSavePath = config.strGroundtruthPath;
    strOriginalFrames = config.strOrigFramePath;
    enumTrackAlgo = config.trackAlgo;
    bPostProcessingGT = config.morphopen;
    strelsize = config.strelsize;

    currState = CONF_FINISHED;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
