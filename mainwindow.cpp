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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionVideo_triggered()
{
    QString filter = tr("Video fiels(*.avi *.mpg  *.webm*.ogg *.ogv );; All files(*.*)");
    QString filePath = QFileDialog::getOpenFileName(this, tr("Import Video"), "", filter);

    gtv = new GTVideo(VIDEO, filePath);

    if ("" != filePath)
    {        
        if (QDir().exists("./source") || QDir().mkdir("./source"))
        {
            videoloader = new LoadVideoThread(filePath);
            connect(videoloader, SIGNAL(completeLoading(bool)), this, SLOT(videoload_completed(bool)), Qt::QueuedConnection);
            videoloader->start();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot find the video/images, or \nCannot create folder ./source in " + QDir::currentPath());
        msgBox.exec();
    }
}

void MainWindow::on_actionImages_triggered()
{

}


void MainWindow::videoload_completed(bool result)
{
    if (!result)
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot create folder ./source in " + QDir::currentPath());
        msgBox.exec();
    }
    else
    {
        qDebug() << "Video/imges have been loaded!\n";
    }
}
