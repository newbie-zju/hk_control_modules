#include "rosthread.h"
#include <iostream>

RosThread::RosThread(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<RecfgMsgVal>("RecfgMsgVal");
    qRegisterMetaType<ImageMsgVal>("ImageMsgVal");
    m_isExt = false; m_ros_thread = NULL;
    n_ = new ros::NodeHandle;
    m_ros_thread = new boost::thread(boost::bind(&RosThread::rosrunThread, this));
}

RosThread::~RosThread()
{
    m_isExt = true;
    
    if(m_ros_thread)
    {
        m_ros_thread->interrupt();
        m_ros_thread->join();
        delete m_ros_thread;
    }
    ROS_INFO("Ros shutdown, proceeding to close the gui.");
}

void RosThread::rosrunThread()
{
    ros::Subscriber sub_data =  n_->subscribe("/hk/data", 10, &RosThread::dataCallback, this);
    ros::Subscriber sub_img =  n_->subscribe("/hk_temperature_video", 10, &RosThread::imgCallback, this);
    ros::Duration initDur(0.2);
    
    while (ros::ok() && !m_isExt)
    {
        ros::spinOnce();
        initDur.sleep();
    }
    ROS_INFO("ros thread closing...");
}

void RosThread::imgCallback(const sensor_msgs::Image::ConstPtr &img_msg)
{   
    emit signal_img_msg(img_msg);
}

void RosThread::dataCallback(const pdt_msgs::hk::ConstPtr &hk_msg)
{
    emit signal_data_msg(hk_msg);
}

