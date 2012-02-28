#ifndef WINDOW_ADDABNORMALRANGE_H
#define WINDOW_ADDABNORMALRANGE_H

#include <QWidget>
#include <QMouseEvent>
#include "gtvideo.h"

class MainWindow;
class roi_window;

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
  //  void on_pushButton_clicked();

//    void on_lineEdit_start_returnPressed();

    void on_spinBox_start_editingFinished();

//    void on_pushButton_2_clicked();

    void on_pushButton_EditROI_clicked();

    void open_roi_window();

    void on_pushButton_done_clicked();

    void on_spinBox_id_valueChanged(int arg1);


private:
    virtual void mousePressEvent(QMouseEvent *e);//when mousePressEvent capture cursor coordinates with respect to
                                                //certain widget
private:
    MainWindow* pMainWindow;
    GTVideo* pGTVideo;
    Ui::window_addAbnormalRange *ui;

    roi_window  *newroiwindow;

    window_addAbnormalRange  *newwindow_addAbnormalRange;

    AbnormalRange* pAbRange;

    const cv::Point ** roipolygon_pts;//array of pointer to roipolygon
    int* roipolygon_npts;

};

#endif // WINDOW_ADDABNORMALRANGE_H
