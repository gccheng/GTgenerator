#include "window_addabnormalrange.h"
#include "ui_window_addabnormalrange.h"

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
