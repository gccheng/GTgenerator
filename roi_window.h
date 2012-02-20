#ifndef ROI_WINDOW_H
#define ROI_WINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include "gtvideo.h"

class window_addAbnormalRange;

namespace Ui {
class roi_window;
}

class roi_window : public QDialog
{
    Q_OBJECT
    
public:
    explicit roi_window(QWidget *parent = 0);
    ~roi_window();
    void InitialSetUp(GTVideo *pGTV, AbnormalRange* pAbR);

private:
    virtual void mousePressEvent(QMouseEvent *e);//when mousePressEvent capture cursor coordinates with respect to
                                                //certain widget
    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    Ui::roi_window *ui;

    window_addAbnormalRange* pAbRangeWindow;

    GTVideo* pGTVideo;

    AbnormalRange* pAbRange;

    QVector<QPoint> ROI_Polygon;
};

#endif // ROI_WINDOW_H
