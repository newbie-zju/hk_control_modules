#pragma once

#include <iostream>
// #include <>
#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QLabel>
#include "rosthread.h"
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ros/package.h>

#define warning_threshold 30

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc, char **argv, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void slot_img(const sensor_msgs::ImageConstPtr &img_msg);
    void slot_data(const pdt_msgs::hkConstPtr &hk_msg);

private:
    Ui::MainWindow *ui;
    QImage m_qImg;
    RosThread *m_rosthread;
    cv::Mat warning_img, src_img;
    std::string normal_path, abnormal_path;
};
