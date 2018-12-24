#ifndef ROSTHREAD_H
#define ROSTHREAD_H

#include <QObject>
#include <QMap>
#include <QMetaType>
#include <boost/thread.hpp>
#include <iostream>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <dynamic_reconfigure/Config.h>
#include <../../devel/include/pdt_msgs/hk.h>
#include <pdt_msgs/hk.h>

typedef pdt_msgs::hk::ConstPtr RecfgMsgVal;
typedef sensor_msgs::Image::ConstPtr ImageMsgVal;

class RosThread : public QObject
{
    Q_OBJECT
public:
    RosThread(QObject *parent = 0);
    ~RosThread();

    void setServicesName(std::string srvname);
    void shutServicesUpdate(std::string srvname);
    cv::Mat src_img;

signals:
    void signal_img_msg(const ImageMsgVal &img_msg);
    void signal_data_msg(const RecfgMsgVal &hk_msg);

private:
    void rosrunThread();
    //void subRecfgCallBack(const ros::MessageEvent<const dynamic_reconfigure::Config> &event);
//     void subRecfgCallBack(const dynamic_reconfigure::Config::ConstPtr &msg, std::string srvname);
    void imgCallback(const sensor_msgs::Image::ConstPtr &img_msg);
    void dataCallback(const pdt_msgs::hk::ConstPtr &hk_msg);

private:
    ros::NodeHandle *n_;
    bool m_isExt;
    boost::thread *m_ros_thread;
    ros::Subscriber m_subrecfg;
//     QMap<std::string, ros::Subscriber> m_subMap;

};

#endif
