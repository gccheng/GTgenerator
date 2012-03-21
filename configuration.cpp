#include "configuration.h"
#include "ui_configuration.h"
#include <QFileDialog>
#include <QMessageBox>

configuration::configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configuration)
{
    ui->setupUi(this);

    ui->comboBox_tracking_algo->addItem("Choose tracking algorithm");
    ui->comboBox_tracking_algo->addItem("Active contour (Snake image)");
    ui->comboBox_tracking_algo->addItem("Coming later...");
    ui->comboBox_tracking_algo->setCurrentIndex(0);
}

configuration::~configuration()
{
    delete ui;
}

void configuration::on_buttonBox_accepted()
{
    QString strGroundtruthPath = ui->edit_groundtruthpath->text();
    QString strOrigFramesPath = ui->edit_origframepath->text();
    bool validIndex = ui->comboBox_tracking_algo->currentIndex() >  0;

    if (!QDir().exists(strGroundtruthPath) || !QDir().exists(strOrigFramesPath) || !validIndex)
    {
        open();
        QMessageBox msgBox;
        msgBox.setText("Error happens in configuration.\n Check the path and parameters.");
        msgBox.exec();
    }
    else
    {
        emit this->configuration_finished(ui->edit_groundtruthpath->text(),
                                          ui->edit_origframepath->text(),
                                          (TrackType)ui->comboBox_tracking_algo->currentIndex());
    }
}

// select folder to save groundtruth results of abnormal objects in the video
void configuration::on_btn_grndtruth_path_released()
{
    // select directory to save groundtruth
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Folder to save groundtruth"),
                                                        QDir::currentPath(),
                                                        QFileDialog::ShowDirsOnly);
    if ("" != dirPath)
    {
        ui->edit_groundtruthpath->setText(dirPath);
    }
}

// select folder to save original frames of the video
void configuration::on_btn_origframes_path_released()
{
    // select directory to save original frames from the video
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Folder to save groundtruth"),
                                                        QDir::currentPath(),
                                                        QFileDialog::ShowDirsOnly);
    if ("" != dirPath)
    {
        ui->edit_origframepath->setText(dirPath);
    }
}

// reset configuration
void configuration::reset()
{
    ui->edit_groundtruthpath->setText("");;
    ui->edit_origframepath->setText("");
    ui->comboBox_tracking_algo->setCurrentIndex(0);
}

void configuration::on_buttonBox_clicked(QAbstractButton *button)
{
    QPushButton *resetButton = reinterpret_cast<QPushButton*>(button);
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
}
