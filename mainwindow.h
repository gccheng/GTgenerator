#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gtvideo.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionVideo_triggered();

    void on_actionImages_triggered();

private:
    Ui::MainWindow *ui;
    GTVideo *gtv;
};

#endif // MAINWINDOW_H
