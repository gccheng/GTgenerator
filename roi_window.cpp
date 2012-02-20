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

void roi_window::InitialSetUp()
{
    int pos = pAbRange->getStart();
    cv::Mat frame_roi = pGTVideo->retrieveFrame(pos);
    QImage img_roi= QImage((const unsigned char*)(frame_roi.data),frame_roi.cols,frame_roi.rows,QImage::Format_RGB888);
    ui->label_roi->setPixmap(QPixmap::fromImage(img_roi));
    ui->label_roi->setScaledContents(true);

}

void roi_window::setGTVideo(GTVideo* pGTV)
{
    pGTVideo=pGTV;
}

void roi_window::setAbRange(AbnormalRange* pAbR)
{
    pAbRange=pAbR;
}

void roi_window::setROIpolygonPointer(const cv::Point** pts,int* npts)
{
    roipolygon_pts=pts;
    roipolygon_npts=npts;
}
void roi_window::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons()==Qt::LeftButton)
    {
        cv::Point pt1=cv::Point(0,0);
        cv::Point pt2=cv::Point(0,0);

        int pos = pAbRange->getStart();
        cv::Mat frame_roi = pGTVideo->retrieveFrame(pos);

        QPoint ccur= QCursor::pos();
        QPoint c = ui->label_roi->mapFromGlobal(ccur);

        int n=ROI_Polygon.size();

        if(c.x()>0&&c.y()>0)
        {
            if(n>0)
            {
            pt1.x=ROI_Polygon.at(n-1).x;
            pt1.y=ROI_Polygon.at(n-1).y;
             }
            pt2.x=double(c.x())/double(ui->label_roi->width())*double(frame_roi.cols);
            pt2.y=double(c.y())/double(ui->label_roi->height())*double(frame_roi.rows);
            ROI_Polygon.push_back(pt2);

            qDebug() << QString("clicked @ x=: %1").arg(pt2.x);
            qDebug() << QString("clicked @ y=: %1").arg(pt2.y);
            n=ROI_Polygon.size();

            if(n>=2)
              {
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
    roi_window::close();
    cv::Mat roi_img(pGTVideo->retrieveFrame(0).rows,pGTVideo->retrieveFrame(0).cols,CV_8UC1);
    roi_img.setTo(0);
    //generate 0-1 mask from polygon input
    const cv::Point * p = ROI_Polygon.data();
    //const cv::Point ** pts = &p;
    roipolygon_pts = &p;
    int npts=ROI_Polygon.size();
    cv::fillPoly(roi_img,roipolygon_pts,&npts,1,cv::Scalar(255,255,255),8,0);
    pAbRange->setROI(roi_img);
    pAbRange->setBoundaryPoints(ROI_Polygon);
    }
}
