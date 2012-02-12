#ifndef WINDOW_ADDABNORMALRANGE_H
#define WINDOW_ADDABNORMALRANGE_H

#include <QWidget>

namespace Ui {
class window_addAbnormalRange;
}

class window_addAbnormalRange : public QWidget
{
    Q_OBJECT
    
public:
    explicit window_addAbnormalRange(QWidget *parent = 0);
    ~window_addAbnormalRange();
    
private:
    Ui::window_addAbnormalRange *ui;
};

#endif // WINDOW_ADDABNORMALRANGE_H
