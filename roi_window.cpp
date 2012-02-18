#include "roi_window.h"
#include "ui_roi_window.h"
#include "window_addabnormalrange.h"

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

roi_window::setAbRangeWindow(window_addAbnormalRange* pAbRW)
{
    pAbRangeWindow = pAbRW;
}



