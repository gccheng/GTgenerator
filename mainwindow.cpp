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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Video"), "", filter);

 /*   if ("" != fileName)
    {
        cv::VideoCapture cap;
        if(!cap.open(fileName.))
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot open video: " + fileName);
        }
        else
        {

        }
    }*/

}
