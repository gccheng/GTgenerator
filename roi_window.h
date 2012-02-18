#ifndef ROI_WINDOW_H
#define ROI_WINDOW_H

#include <QDialog>

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
    void setAbRangeWindow(window_addAbnormalRange* pAbRW);

private:
    Ui::roi_window *ui;

    window_addAbnormalRange* pAbRangeWindow;
};

#endif // ROI_WINDOW_H
