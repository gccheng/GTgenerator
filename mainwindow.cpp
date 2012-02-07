#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

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
        cv::VideoCapture cap;
        if(!cap.open(filePath.toUtf8().constData()))
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot open video: " + filePath);
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
                filename.sprintf("%03d.tif", frameno);
                cv::imwrite(filename.toUtf8().constData(), frame);
            }
        }
    }

}

void MainWindow::on_actionImages_triggered()
{

}
