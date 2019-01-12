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
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ros/package.h>

#define save_by_day_flag 0

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc, char **argv, QWidget *parent = 0);
    ~MainWindow();
   
    
private:
    void loadParams();
    void hk_write_temperature(std::string &now_time, int preset_position, int preset_id,
                        std::string &current_position, float now_temperature);

private slots:
    void on_pushButton_clicked();
    void on_pushButton2_clicked();
    void slot_img(const ImageMsgVal &img_msg);
    void slot_data(const RecfgMsgVal &hk_msg);

private:
    Ui::MainWindow *ui;
    QImage m_qImg;
    RosThread *m_rosthread;
    cv::Mat warning_img, src_img;
    std::string normal_path, abnormal_path, image_save_path;
    std::string normal_output_file, abnormal_output_file;
    std::string pkg_path;
    float warning_threshold;
    std::vector<std::string> show_position;
    int preset_position_sum, ID_sum;
    std::vector<std::string> show_position_data_vec;
    int first_position_num, save_img_count;
    bool save_img_flag;
};
