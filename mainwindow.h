#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gtvideo.h"
#include "loadvideothread.h"

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

    void videoload_completed(bool result);


    //void on_horizontalSlider_sliderMoved(int position);

    void on_Slider_videoloaded_sliderMoved(int position);

    void on_Slider_videoloaded_sliderPressed();

    void on_Slider_videoloaded_valueChanged(int value);

    void on_Slider_videoloaded_sliderReleased();

    void on_actionAddBoundary_triggered();

private:
    Ui::MainWindow *ui;
    GTVideo *gtv;
    LoadVideoThread *videoloader;
};

#endif // MAINWINDOW_H
