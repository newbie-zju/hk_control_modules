#include "mainwindow.h"
#include "main_window/ui_mainwindow.h"
#include <QFileDialog>
#include <XmlRpcClient.h>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <string>

MainWindow::MainWindow(int argc, char **argv, QWidget *parent) :
<<<<<<< HEAD
    QMainWindow(parent), warning_threshold(20), preset_position_sum(0), ID_sum(0),
    ui(new Ui::MainWindow), first_position_num(0), save_img_count(0), save_img_flag(false)
=======
    QMainWindow(parent), warning_threshold(20), preset_position_num(0), ID_num(0),
    ui(new Ui::MainWindow)
>>>>>>> e78648da6ae4f52fc42ef6098b02397a1799a056
{
    ui->setupUi(this);
    this->setWindowTitle(QString("UI"));
//     ui->label_7->setFrameShape (QFrame::Box);
//     ui->label_7->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
//     ui->label_8->setFrameShape (QFrame::Box);
//     ui->label_8->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
//     ui->label_9->setFrameShape (QFrame::Box);
//     ui->label_9->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
//     ui->label_10->setFrameShape (QFrame::Box);
//     ui->label_10->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
//     ui->label_11->setFrameShape (QFrame::Box);
//     ui->label_11->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
//     ui->label_12->setFrameShape (QFrame::Box);
//     ui->label_12->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");
//     ui->label_15->setFrameShape (QFrame::Box);
//     ui->label_15->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 0, 0);");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton2_clicked()));

    QFont font("Microsoft YaHei", 16, 50);
    ui->label_15->setFont(font);
    ui->label_16->setFont(font);
    ui->label_17->setFont(font);
    ui->label_18->setFont(font);

    ui->label_35->setFont(font);
    ui->label_36->setFont(font);
    ui->label_37->setFont(font);
    ui->label_38->setFont(font);

    ui->label_43->setFont(font);
    ui->label_44->setFont(font);
    ui->label_45->setFont(font);
    ui->label_46->setFont(font);

    ui->label_47->setFont(font);
    ui->label_48->setFont(font);
    ui->label_49->setFont(font);
    ui->label_50->setFont(font);

    ui->label_51->setFont(font);
    ui->label_52->setFont(font);
    ui->label_53->setFont(font);
    ui->label_54->setFont(font);

    ui->label_55->setFont(font);
    ui->label_56->setFont(font);
    ui->label_57->setFont(font);
    ui->label_58->setFont(font);

    ui->label_59->setFont(font);
    ui->label_60->setFont(font);
    ui->label_61->setFont(font);
    ui->label_62->setFont(font);

    ui->label_63->setFont(font);
    ui->label_64->setFont(font);
    ui->label_65->setFont(font);
    ui->label_66->setFont(font);

    ui->label_67->setFont(font);
    ui->label_68->setFont(font);
    ui->label_69->setFont(font);
    ui->label_70->setFont(font);

    ui->label_7->setFont(font);

//     ui->label_7->setText(QString::fromStdString("-1"));
//     ui->label_8->setText(QString::fromStdString("-1"));
//     ui->label_9->setText(QString::fromStdString("-1"));
//     ui->label_10->setText(QString::fromStdString("-1"));
//     ui->label_15->setText(QString::fromStdString("-1"));
    
    ros::init(argc, argv, "main_window");
    m_rosthread = new RosThread;
    pkg_path = ros::package::getPath("main_window");
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

    if(save_img_flag)  save_img_count = save_img_count + 1;
    if (save_img_count == 10) 
    {
        save_img_count = 0;
        save_img_flag = false;
        cv::imwrite(image_save_path, src_img);

    }
}

void MainWindow::slot_data(const RecfgMsgVal &hk_msg)
<<<<<<< HEAD
{ 
    int pre_position_num, ID_num;
    std::string data_time_tmp = hk_msg->data_time;
=======
{
    ui->label_7->setText(QString::fromStdString(hk_msg->data_time));
    ui->label_8->setText(QString::fromStdString(hk_msg->pre_position));
    ui->label_9->setText(QString::fromStdString(hk_msg->ID));
    ui->label_10->setText(QString::fromStdString(hk_msg->temperature));
    
    int pre_position_num, ID_num;
>>>>>>> e78648da6ae4f52fc42ef6098b02397a1799a056
    std::stringstream stream0(hk_msg->pre_position);  
    stream0 >> pre_position_num;
    std::stringstream stream1(hk_msg->ID);  
    stream1 >> ID_num;
<<<<<<< HEAD

    ui->label_7->setText(QString::fromStdString(data_time_tmp));
    //std_msgs::Bool change_mode_msgs;
   // if (pre_position_num == 6)
   //     change_mode_msgs.data = true;
   // else
   //     change_mode_msgs.data = false;
  //  m_rosthread->m_pubstate.publish(change_mode_msgs);
    if (first_position_num != pre_position_num)
    {
        first_position_num = pre_position_num;
	    show_position_data_vec.clear();
    }
    
    std::string position_name_str, full_information;
    int position_name_str_index;
    position_name_str_index = ID_sum*(pre_position_num - 1) + ID_num - 1;
    if (position_name_str_index > show_position.size())
    {
        printf("error ID and pre_position\n");
	    position_name_str = "error ID and pre_position";
    }
    else
    {
        position_name_str = show_position[position_name_str_index];
    }
	
   // full_information = hk_msg->pre_position + "          " 
    //                  + hk_msg->ID + "            " 
    //                  + position_name_str + "              " 
    ///                  + hk_msg->temperature;
    show_position_data_vec.push_back(hk_msg->pre_position);
    show_position_data_vec.push_back(hk_msg->ID);
    show_position_data_vec.push_back(position_name_str);
    show_position_data_vec.push_back(hk_msg->temperature);
    
    for (int i = 0; i < show_position_data_vec.size(); i = i + 4)
    {
       if (i == 0)
       {
           ui->label_15->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_16->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_17->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_18->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
       else  if (i == 4)
       {
           ui->label_35->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_36->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_37->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_38->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
        else  if (i == 8)
       {
           ui->label_43->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_44->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_45->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_46->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
        else  if (i == 12)
       {
           ui->label_47->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_48->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_49->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_50->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
        else  if (i == 16)
       {
           ui->label_51->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_52->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_53->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_54->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
        else  if (i == 20)
       {
           ui->label_55->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_56->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_57->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_58->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
        else  if (i == 24)
       {
           ui->label_59->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_60->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_61->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_62->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
        else  if (i == 28)
       {
           ui->label_63->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_64->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_65->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_66->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
        else  if (i == 32)
       {
           ui->label_67->setText(QString::fromStdString(show_position_data_vec[i]));
           ui->label_68->setText(QString::fromStdString(show_position_data_vec[i+1]));
           ui->label_69->setText(QString::fromStdString(show_position_data_vec[i+2]));
           ui->label_70->setText(QString::fromStdString(show_position_data_vec[i+3]));
       }
       
     
    }

    // warning image
=======
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
    
>>>>>>> e78648da6ae4f52fc42ef6098b02397a1799a056
    float temper_tmp;
    std::istringstream is(hk_msg->temperature);
    is >> temper_tmp;
    
    if (warning_threshold == 0)
    {
        printf("warning_threshold is 0, set it to Windows default\n");
        warning_threshold = 20;
    }
    hk_write_temperature(data_time_tmp, pre_position_num, ID_num, position_name_str, temper_tmp);

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
    config_file = pkg_path + "/config/position.yaml";
    printf("param's file path: %c\n", config_file.c_str());
    
    cv::FileStorage fsSettings(config_file, cv::FileStorage::READ);
    if(!fsSettings.isOpened())
    {
        std::cerr << "ERROR: Wrong path to settings" << std::endl;
	exit(1);
    }
    
    preset_position_sum = fsSettings["preset_position_sum"];
    ID_sum = fsSettings["ID_sum"];
    
    if (preset_position_sum == 0 || ID_sum == 0)
    {
        printf("error position num\n");
	exit(1);
    }
    
    for (size_t i = 0; i < preset_position_sum; i++)
    {
        for (size_t j = 0; j < ID_sum; j++)
	{
	    std::string position_str_tmp, list_name_str;
	    list_name_str = std::to_string(i+1) + "_" + std::to_string(j+1);
	    fsSettings[list_name_str] >> position_str_tmp;
	    show_position.push_back(position_str_tmp);
	}
    }
    
    fsSettings.release();

}

void MainWindow::hk_write_temperature(std::string &now_time, int preset_position, int preset_id,
                                    std::string &current_position, float now_temperature)
{   
    std::string day_str;
    if (save_by_day_flag)  day_str = now_time.substr(0, 10);
    else  day_str = now_time.substr(11, 5);
    //std::cout << "day_str" << day_str << std::endl;
    normal_output_file = "/home/gjdw/Desktop/wl/所有记录/" + day_str + ".txt";
    abnormal_output_file = "/home/gjdw/Desktop/wl/异常记录/报表/" + day_str + ".txt";

    std::cout << "day_str" << normal_output_file << std::endl;
    //std::cout << "normal_output_file" << normal_output_file << std::endl;
    // normal_file
    std::ifstream ifile1, ifile2;
    ifile1.open(normal_output_file);
    if(!ifile1)
    {
        std::cout << "cannot find: " << normal_output_file << std::endl;
        std::ofstream ofile;
        ofile.open(normal_output_file);
        ofile << std::endl;
        ofile.close();
        ifile1.open(normal_output_file);
        if(!ifile1)
        {
            std::cout << "failed" << std::endl;
            return;
        }
        else
        {
            std::cout << "sucess" << std::endl;
            ifile1.close();
        }
    }

    std::ofstream ofile1, ofile2;
    ofile1.open(normal_output_file, std::ios::app);
    if (now_temperature < warning_threshold)
    {
        ofile1<<"时间:" << now_time << ", " <<"预置点:" << preset_position << ", " << "ID:"<< preset_id << ", " << "位置: " 
        << current_position << ", " << "温度:"<< now_temperature << std::endl;
    }
    else
    {
<<<<<<< HEAD
          ofile1<<"时间:" << now_time << ", " <<"预置点:" << preset_position << ", " << "ID:"<< preset_id << ", " << "位置: " 
        << current_position << ", " << "温度:"<< now_temperature << ", " << "异常" << std::endl;
=======
	warning_img = cv::imread(abnormal_path);
	cv::resize(warning_img, warning_img, cv::Size(640, 360));

	cv::putText(warning_img, hk_msg->data_time, cv::Point(260,75), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	cv::putText(warning_img, hk_msg->pre_position, cv::Point(280,122), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	cv::putText(warning_img, hk_msg->ID, cv::Point(235,175), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	cv::putText(warning_img, hk_msg->temperature, cv::Point(260,220), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 255), 2);
	
	cv::cvtColor(warning_img, warning_img, CV_BGR2RGB);
	m_qImg = QImage((const unsigned char*)(warning_img.data), warning_img.cols, warning_img.rows, QImage::Format_RGB888);
	ui->label_11->setPixmap(QPixmap::fromImage(m_qImg));
>>>>>>> e78648da6ae4f52fc42ef6098b02397a1799a056
    }
    
    // abnormal_file
    if (now_temperature > warning_threshold)
    {
        ifile2.open(abnormal_output_file);
        if(!ifile2)
        {
            std::cout << "cannot find: " << abnormal_output_file << std::endl;
            std::ofstream ofile;
            ofile.open(abnormal_output_file);
            ofile << std::endl;
            ofile.close();
            ifile2.open(abnormal_output_file);
            if(!ifile2)
            {
                std::cout << "failed" << std::endl;
                return;
            }
            else
            {
                std::cout << "sucess" << std::endl;
                ifile2.close();
            }
        }

        ofile2.open(abnormal_output_file, std::ios::app);
        ofile2<<"时间:" << now_time << ", " <<"预置点:" << preset_position << ", " << "ID:"<< preset_id << ", " << "位置: " << current_position << ", " << "温度:"<< now_temperature 
        << ", " << "温度异常" << std::endl;
        if (!src_img.empty()) 
        {
            save_img_flag = true;
            image_save_path = "/home/gjdw/Desktop/wl/异常记录/红外图像/" + current_position + ".png";
        }
    }

    printf("record the temerature\n");
    std::cout<<"时间:" << now_time << ", " <<"预置点:" << preset_position << ", " << "ID:"<< preset_id << ", " << "温度:"<< now_temperature << std::endl;


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






















