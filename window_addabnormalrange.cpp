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

void window_addAbnormalRange::on_pushButton_clicked()
{

   qDebug() << "push button clicked";
   int framecount= pMainWindow->getGTVideo()->getFrameCount();
   qDebug() << QString("Frame count from mainwindow: %1").arg(framecount);


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

void window_addAbnormalRange::on_lineEdit_start_returnPressed()
{

}

void window_addAbnormalRange::on_spinBox_start_editingFinished()
{
    int frame_id=ui->spinBox_start->value();
    cv::Mat frame = pGTVideo->retrieveFrame(frame_id);
    QImage img= QImage((const unsigned char*)( frame.data),frame.cols,frame.rows,QImage::Format_RGB888);
    ui->label_frame->setPixmap(QPixmap::fromImage(img));
    ui->label_frame->setScaledContents(true);

}

void window_addAbnormalRange::open_roi_window()
{

    cv::Mat roi_ini=pGTVideo->retrieveFrame(0);
    roi_ini.setTo(1);

    //AbnormalRange newAbRange= new AbnormalRange(0,0,roi_ini);
    //default start=0, end=0, ROI at default is the whole image
    pAbRange = new AbnormalRange(0,0,roi_ini);
    pAbRange->setStartEnd(uint(ui->spinBox_start->value()),uint(ui->spinBox_end->value()));
    pAbRange->setROI(roi_ini);

    //gtv->addAbnormalRange(*newAbRange);

    newroiwindow= new roi_window();
    newroiwindow->InitialSetUp(pGTVideo,pAbRange);
    newroiwindow->show();


}

void window_addAbnormalRange::on_pushButton_2_clicked()
{

}

void window_addAbnormalRange::on_pushButton_EditROI_clicked()
{
    open_roi_window();


}
