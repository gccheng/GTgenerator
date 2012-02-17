#ifndef WINDOW_ADDABNORMALRANGE_H
#define WINDOW_ADDABNORMALRANGE_H

#include <QWidget>
#include <QMouseEvent>
#include "gtvideo.h"

class MainWindow;

namespace Ui {
class window_addAbnormalRange;
}

class window_addAbnormalRange : public QWidget
{
    Q_OBJECT
    
public:
    explicit window_addAbnormalRange(QWidget *parent = 0);
    ~window_addAbnormalRange();
    void setMainWindow(MainWindow* pMW);
    void setGTVideo(GTVideo* pGTV);

    
private slots:
    void on_pushButton_clicked();

    void on_lineEdit_start_returnPressed();

    void on_spinBox_start_editingFinished();

private:
    virtual void mousePressEvent(QMouseEvent *e);//when mousePressEvent capture cursor coordinates with respect to
                                                //certain widget
private:
    MainWindow* pMainWindow;
    GTVideo* pGTVideo;
    Ui::window_addAbnormalRange *ui;

    QVector<cv::Point> ROI_Polygon;                // frames of the input video

};

#endif // WINDOW_ADDABNORMALRANGE_H
