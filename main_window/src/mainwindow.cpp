#include "mainwindow.h"
#include "main_window/ui_mainwindow.h"
#include <QFileDialog>
#include <XmlRpcClient.h>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <string>

MainWindow::MainWindow(int argc, char **argv, QWidget *parent) :
    QMainWindow(parent), warning_threshold(20), preset_position_num(0), ID_num(0),
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
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton2_clicked()));
    ui->label_7->setText(QString::fromStdString("-1"));
    ui->label_8->setText(QString::fromStdString("-1"));
    ui->label_9->setText(QString::fromStdString("-1"));
    ui->label_10->setText(QString::fromStdString("-1"));
    
    ros::init(argc, argv, "main_window");
    m_rosthread = new RosThread;
    std::string pkg_path = ros::package::getPath("main_window");
    normal_path = pkg_path + "/config/"+ "normal.jpg";
    abnormal_path = pkg_path + "/config/"+ "abnormal.jpg";
    connect(m_rosthread, SIGNAL(signal_img_msg(ImageMsgVal)), this, SLOT(slot_img(ImageMsgVal)), Qt::QueuedConnection);
    connect(m_rosthread, SIGNAL(signal_data_msg(RecfgMsgVal)), this, SLOT(slot_data(RecfgMsgVal)), Qt::QueuedConnection);
    
    loadParams();

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

void MainWindow::on_pushButton2_clicked()
{
    QString mString = ui->lineEdit->text();
    warning_threshold = mString.toFloat();
}

void MainWindow::slot_img(const ImageMsgVal &img_msg)
{
    cv::Mat showImg;
    cv_bridge::CvImageConstPtr ptr;
    ptr = cv_bridge::toCvCopy(img_msg, sensor_msgs::image_encodings::BGR8);
    src_img = ptr->image.clone();
    showImg = src_img.clone();
    cv::resize(showImg, showImg, cv::Size(640, 360));
    cv::cvtColor(showImg, showImg, CV_BGR2RGB);
    m_qImg = QImage((const unsigned char*)(showImg.data), showImg.cols, showImg.rows, QImage::Format_RGB888);
    ui->label_12->setPixmap(QPixmap::fromImage(m_qImg));
}

void MainWindow::slot_data(const RecfgMsgVal &hk_msg)
{
    ui->label_7->setText(QString::fromStdString(hk_msg->data_time));
    ui->label_8->setText(QString::fromStdString(hk_msg->pre_position));
    ui->label_9->setText(QString::fromStdString(hk_msg->ID));
    ui->label_10->setText(QString::fromStdString(hk_msg->temperature));
    
    int pre_position_num, ID_num;
    std::stringstream stream0(hk_msg->pre_position);  
    stream0 >> pre_position_num;
    std::stringstream stream1(hk_msg->ID);  
    stream1 >> ID_num;
    if (ID_num*pre_position_num > show_position.size())
    {
        printf("error ID and pre_position\n");
	exit(1);
    }
    else
    {
	std::string position_name_str;
	position_name_str = show_position[ID_num*pre_position_num - 1];
	
	ui->label_15->setText(QString::fromStdString(position_name_str));
    }
    
    float temper_tmp;
    std::istringstream is(hk_msg->temperature);
    is >> temper_tmp;

    if (warning_threshold == 0)
    {
	printf("warning_threshold is 0, set it to Windows default\n");
	warning_threshold = 20;
    }
    if (temper_tmp < warning_threshold)
    {
	warning_img = cv::imread(normal_path);
	cv::resize(warning_img, warning_img, cv::Size(640, 360));
	cv::cvtColor(warning_img, warning_img, CV_BGR2GRAY);
	m_qImg = QImage((const unsigned char*)(warning_img.data), warning_img.cols, warning_img.rows, QImage::Format_Indexed8);
	ui->label_11->setPixmap(QPixmap::fromImage(m_qImg));
    }
    else
    {
	warning_img = cv::imread(abnormal_path);
	cv::resize(warning_img, warning_img, cv::Size(640, 360));

	cv::putText(warning_img, hk_msg->data_time, cv::Point(260,75), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	cv::putText(warning_img, hk_msg->pre_position, cv::Point(280,122), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	cv::putText(warning_img, hk_msg->ID, cv::Point(235,175), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	cv::putText(warning_img, hk_msg->temperature, cv::Point(260,220), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	
	cv::cvtColor(warning_img, warning_img, CV_BGR2RGB);
	m_qImg = QImage((const unsigned char*)(warning_img.data), warning_img.cols, warning_img.rows, QImage::Format_RGB888);
	ui->label_11->setPixmap(QPixmap::fromImage(m_qImg));
    }
}

void MainWindow::loadParams()
{
    std::string config_file;
    std::string pkg_path = ros::package::getPath("main_window");
    config_file = pkg_path + "/config/position.yaml";
    printf("param's file path: %c\n", config_file.c_str());
    
    cv::FileStorage fsSettings(config_file, cv::FileStorage::READ);
    if(!fsSettings.isOpened())
    {
        std::cerr << "ERROR: Wrong path to settings" << std::endl;
	exit(1);
    }
    
    preset_position_num = fsSettings["preset_position_num"];
    ID_num = fsSettings["ID_num"];
    
    if (preset_position_num == 0 || ID_num == 0)
    {
        printf("error position num\n");
	exit(1);
    }
    
    for (size_t i = 0; i < preset_position_num; i++)
    {
        for (size_t j = 0; j < ID_num; j++)
	{
	    std::string position_str_tmp, list_name_str;
	    list_name_str = std::to_string(i+1) + "_" + std::to_string(j+1);
	    fsSettings[list_name_str] >> position_str_tmp;
	    show_position.push_back(position_str_tmp);
	}
    }
    
    fsSettings.release();

}





















