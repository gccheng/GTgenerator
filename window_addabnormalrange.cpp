#include "window_addabnormalrange.h"
#include "ui_window_addabnormalrange.h"

#include "mainwindow.h"
#include <QDebug>
#include "roi_window.h"

window_addAbnormalRange::window_addAbnormalRange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::window_addAbnormalRange)
{
    ui->setupUi(this);
}


window_addAbnormalRange::~window_addAbnormalRange()
{
    delete ui;
}

void window_addAbnormalRange::setMainWindow(MainWindow* pMW)
{
    pMainWindow = pMW;
}

void window_addAbnormalRange::setGTVideo(GTVideo *pGTV)
{
    pGTVideo = pGTV;

    ui->spinBox_id->setValue(pGTVideo->getAbnormallistsize()+1);

}
void window_addAbnormalRange::mousePressEvent(QMouseEvent *e)
{

    if(e->buttons()==Qt::LeftButton)
    {
        QPoint ccur= QCursor::pos();
        QPoint c = ui->label_frame->mapFromGlobal(ccur);
        qDebug() << QString("clicked @ x=: %1").arg(c.x());
        qDebug() << QString("clicked @ y=: %1").arg(c.y());
    }
}

void window_addAbnormalRange::on_spinBox_start_editingFinished()
{
    int frame_id = ui->spinBox_start->value();
    cv::Mat frame = pGTVideo->retrieveFrame(frame_id);

    //to add contour of pAbRange
    if (pGTVideo->getAbnormallistsize()>0)
    {
        if(pAbRange->getStart()==ui->spinBox_start->value())
          {
           cv::polylines(frame,roipolygon_pts,roipolygon_npts,1,1,cv::Scalar(255,255,255),8,0);
          }
    }
    QImage img= QImage((const unsigned char*)(frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
    ui->label_frame->setPixmap(QPixmap::fromImage(img));
    ui->label_frame->setScaledContents(true);

}

void window_addAbnormalRange::open_roi_window()
{
    newroiwindow= new roi_window();

    cv::Mat roi_ini(pGTVideo->retrieveFrame(0).rows,pGTVideo->retrieveFrame(0).cols,CV_8UC1);
    roi_ini.setTo(0);

    //AbnormalRange newAbRange = new AbnormalRange(0,0,roi_ini);
    //default start=0, end=0, ROI at default is the whole image

    pAbRange = new AbnormalRange(0,0,roi_ini);
    pAbRange->setStartEnd(uint(ui->spinBox_start->value()),uint(ui->spinBox_end->value()));
    pAbRange->setROI(roi_ini);

    newroiwindow->setGTVideo(pGTVideo);
    newroiwindow->setAbRange(pAbRange);
    newroiwindow->setROIpolygonPointer(roipolygon_pts,roipolygon_npts);
    newroiwindow->InitialSetUp();
    newroiwindow->show();
}

void window_addAbnormalRange::on_pushButton_EditROI_clicked()
{
    open_roi_window();
}

void window_addAbnormalRange::on_pushButton_done_clicked()
{
    pAbRange->setStartEnd(uint(ui->spinBox_start->value()),uint(ui->spinBox_end->value()));

    if(ui->spinBox_id->value()==pGTVideo->getAbnormallistsize())
    {
      pGTVideo->updateAbnormalRange(*pAbRange);
    }
    else
    {
      pGTVideo->addAbnormalRange(*pAbRange);
    }

}

void window_addAbnormalRange::on_spinBox_id_valueChanged(int arg1)
{

}
