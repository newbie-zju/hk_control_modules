#include "mainwindow.h"
#include "main_window/ui_mainwindow.h"
#include <QFileDialog>
#include <XmlRpcClient.h>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <string>

MainWindow::MainWindow(int argc, char **argv, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("UI"));
    ui->label_7->setFrameShape (QFrame::Box);
    ui->label_7->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
    ui->label_8->setFrameShape (QFrame::Box);
    ui->label_8->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
    ui->label_9->setFrameShape (QFrame::Box);
    ui->label_9->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
    ui->label_10->setFrameShape (QFrame::Box);
    ui->label_10->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
    ui->label_11->setFrameShape (QFrame::Box);
    ui->label_11->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
    ui->label_12->setFrameShape (QFrame::Box);
    ui->label_12->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    ui->label_7->setText(QString::fromStdString("-1"));
    ui->label_8->setText(QString::fromStdString("-1"));
    ui->label_9->setText(QString::fromStdString("-1"));
    ui->label_10->setText(QString::fromStdString("-1"));
    
    ros::init(argc, argv, "main_window");
    m_rosthread = new RosThread;
    std::string pkg_path = ros::package::getPath("image_to_bag");
    normal_path = pkg_path + "/config/"+ "normal.jpg";
    abnormal_path = pkg_path + "/config/"+ "abnormal.jpg";
    connect(m_rosthread, SIGNAL(signal_img_msg(pdt_msgs::hk)), this, SLOT(slot_img(pdt_msgs::hk)), Qt::QueuedConnection);
    connect(m_rosthread, SIGNAL(signal_data_msg(pdt_msgs::hk)), this, SLOT(slot_data(pdt_msgs::hk)), Qt::QueuedConnection);
 
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_rosthread;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}

void MainWindow::slot_img(const sensor_msgs::ImageConstPtr &img_msg)
{
      cv_bridge::CvImageConstPtr ptr;
      ptr = cv_bridge::toCvCopy(img_msg, sensor_msgs::image_encodings::MONO8);
      src_img = ptr->image.clone();
//     cv::cvtColor(showImg, showImg, CV_BGR2RGB);
      m_qImg = QImage((const unsigned char*)(src_img.data), src_img.cols, src_img.rows, QImage::Format_RGB888);
      ui->label_12->setPixmap(QPixmap::fromImage(m_qImg));
}

void MainWindow::slot_data(const pdt_msgs::hkConstPtr &hk_msg)
{
      ui->label_7->setText(QString::fromStdString(hk_msg->data_time));
      ui->label_8->setText(QString::fromStdString(hk_msg->pre_position));
      ui->label_9->setText(QString::fromStdString(hk_msg->ID));
      ui->label_10->setText(QString::fromStdString(hk_msg->temperature));
      
      float temper_tmp;
      std::istringstream is("12");
      is >> temper_tmp;

      if (temper_tmp < warning_threshold)
      {
          warning_img = cv::imread(normal_path);
      }
      else
      {
	  warning_img = cv::imread(abnormal_path);
	  cv::putText(warning_img, hk_msg->data_time, cv::Point(50,60), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
	  cv::putText(warning_img, hk_msg->pre_position, cv::Point(80,60), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
	  cv::putText(warning_img, hk_msg->ID, cv::Point(110,60), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
	  cv::putText(warning_img, hk_msg->temperature, cv::Point(140,60), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
      }
      cv::cvtColor(warning_img, warning_img, CV_BGR2RGB);
      m_qImg = QImage((const unsigned char*)(warning_img.data), warning_img.cols, warning_img.rows, QImage::Format_RGB888);
      ui->label_11->setPixmap(QPixmap::fromImage(m_qImg));
}





















