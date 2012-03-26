#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/*
 * Description: Class configuration sets up the parameters needed
 *              by the program, such as tracking algorithm used,
 *              path to save the results, path to save the original
 *              frame of the video, among others
 * Create Date: 03/21/2012
 * Last Update: ##/##/####: ************
 *
 */

#include <QDialog>
#include <QAbstractButton>

#include "sharedtypes.h"
#include "configparam.h"

namespace Ui {
    class configuration;
}

class configuration : public QDialog
{
    Q_OBJECT

public:
    explicit configuration(QWidget *parent = 0);
    ~configuration();

 signals:
    /*void configuration_finished(QString gtPath, QString origframePath, TrackType trackAlgo);*/
    void configuration_finished(ConfigParam config);

private slots:
    void on_buttonBox_accepted();

    void on_btn_grndtruth_path_released();

    void on_btn_origframes_path_released();

    void on_buttonBox_clicked(QAbstractButton *button);

    void reset();

private:
    Ui::configuration *ui;
};

#endif // CONFIGURATION_H
