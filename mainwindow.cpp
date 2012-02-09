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

        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot create folder ./source in " + QDir::currentPath());
            msgBox.exec();
        }
    }
}

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
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot create folder ./source in " + QDir::currentPath());
            msgBox.exec();
        }
    }
}


void MainWindow::videoload_completed(bool result)
{
    if (!result)
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot load video");
        msgBox.exec();
    }
    else
    {
        qDebug() << QString("Frames loaded: %1").arg(gtv->getFrameNumber());
        qDebug() << "Video/imges have been loaded!\n";
        ui->actionVideo->setDisabled(true);
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{

}
