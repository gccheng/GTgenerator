#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMetaMethod>

#include "gtvideo.h"
#include "loadvideothread.h"

class window_addAbnormalRange;
class configuration;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GTVideo* getGTVideo() const;
    void setState(States type);

private:
    void setupTimer(int interval, char method);
    void stopTimer();

private slots:
    void updateSlider();
    void on_actionVideo_triggered();
    void on_actionImages_triggered();

    void videoload_completed(bool result);

    void open_window_addabnormalrange();


    //void on_horizontalSlider_sliderMoved(int position);

    void on_Slider_videoloaded_sliderMoved(int position);

    void on_Slider_videoloaded_sliderPressed();

    void on_Slider_videoloaded_valueChanged(int value);

    void on_Slider_videoloaded_sliderReleased();

    void on_actionAddBoundary_triggered();

    void on_Button_prev_clicked();

    void on_actionGroundtruth_2_triggered();

    void on_actionConfiguration_triggered();

    void set_configuration(QString gtPath, QString origframePath, TrackType trackAlgo);

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    GTVideo *gtv;                   // the data model and operations
    LoadVideoThread *videoloader;   // thread to load videos
    QTimer *timer;                  // timer for operation during video-loading...
    window_addAbnormalRange  *newwindow_addAbnormalRange;

    QString strGroundtruthSavePath; // folder to save generated groundtruth images
    QString strOriginalFrames;      // folder to save original frames of the video
    TrackType enumTrackAlgo;        // algorithm for tracking

    States currState;               // current state of the processing
};

#endif // MAINWINDOW_H
