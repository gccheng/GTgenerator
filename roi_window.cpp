#include "roi_window.h"
#include "ui_roi_window.h"
#include "window_addabnormalrange.h"

#include <QDebug>
#include <QtGui>
#include <QtCore>

roi_window::roi_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::roi_window)
{
    ui->setupUi(this);
}

roi_window::~roi_window()
{
    delete ui;
}

void roi_window::InitialSetUp(GTVideo* pGTV, AbnormalRange* pAbR)
{
    pGTVideo=pGTV;
    pAbRange=pAbR;
    int pos = pAbRange->getStart();
    cv::Mat frame_roi = pGTVideo->retrieveFrame(pos);
    QImage img_roi= QImage((const unsigned char*)(frame_roi.data),frame_roi.cols,frame_roi.rows,QImage::Format_RGB888);
    ui->label_roi->setPixmap(QPixmap::fromImage(img_roi));
    ui->label_roi->setScaledContents(true);
}

void roi_window::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons()==Qt::LeftButton)
    {
        int pos = pAbRange->getStart();
        cv::Mat frame_roi = pGTVideo->retrieveFrame(pos);

        QPoint ccur= QCursor::pos();
        QPoint c = ui->label_roi->mapFromGlobal(ccur);

        cv::Point pt1=cv::Point(0,0);
        cv::Point pt2=cv::Point(0,0);

        if(c.x()>0&&c.y()>0)
        {
           ROI_Polygon.push_back(c);
           int n=ROI_Polygon.size();
           if(n>=2)
              {
               //to draw a line connecting the last two points
               pt1.x=double(ROI_Polygon.at(n-2).x())/double(ui->label_roi->width())*double(frame_roi.cols);
               pt1.y=double(ROI_Polygon.at(n-2).y())/double(ui->label_roi->height())*double(frame_roi.rows);
               pt2.x=double(ROI_Polygon.at(n-1).x())/double(ui->label_roi->width())*double(frame_roi.cols);
               pt2.y=double(ROI_Polygon.at(n-1).y())/double(ui->label_roi->height())*double(frame_roi.rows);

               int thickness= double(ui->label_roi->width())/double(frame_roi.cols);
               cv::line(frame_roi,pt1,pt2,cv::Scalar(0,255,0),thickness,8,0);
              }
           QImage img_roi= QImage((const unsigned char*)(frame_roi.data),frame_roi.cols,frame_roi.rows,QImage::Format_RGB888);
           ui->label_roi->setPixmap(QPixmap::fromImage(img_roi));
           ui->label_roi->setScaledContents(true);
        }
    }
}

void roi_window::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->buttons()==Qt::LeftButton)
    {
    QPoint ccur= QCursor::pos();
    QPoint c = ui->label_roi->mapFromGlobal(ccur);
    ROI_Polygon.push_back(c);
    }
    roi_window::close();
}
