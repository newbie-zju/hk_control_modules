#include "hk_control.h"


float P, I, D;
float keep_static_threshold, joy_sensitivity;
int time_threshold;
std::string sub_box_topic;
ros::Publisher pub_data;

hk_control::Hk_Control my_hk_control;

void get_ros_param(ros::NodeHandle &nh_param);

void boxCallback(const pdt_msgs::BoundingBox::ConstPtr &box_msg)
{
	float target_center_x, target_center_y;
	std_msgs::Header detect_timestamp;
	detect_timestamp = box_msg->header;
	target_center_x = (box_msg->xmax_normal+box_msg->xmin_normal)/2;
	target_center_y = (box_msg->ymin_normal+box_msg->ymax_normal)/2;
// 	std::cout << "target_center_x" << target_center_x << std::endl;
// 	std::cout << "target_center_y" << target_center_y << std::endl;
	my_hk_control.hk_control_process(target_center_x, target_center_y, detect_timestamp);
}

void joyCallback(const sensor_msgs::JoyConstPtr &joy_msg)
{
	if (joy_msg->buttons[0] || joy_msg->buttons[1])
	{
		my_hk_control.a_manul = joy_msg->buttons[0];
		my_hk_control.b_auto = joy_msg->buttons[1];
	}
	my_hk_control.ax = joy_msg->axes[0];
	my_hk_control.ay = joy_msg->axes[1];
	if (joy_msg->buttons[2])
	    my_hk_control.az = 1;
}

void imageCb(const sensor_msgs::ImageConstPtr &msg) 
{
    cv::Mat img_tmp;
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception &e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv_ptr->image.copyTo(img_tmp);
    my_hk_control.src_img = img_tmp.clone();
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "svm_train_node");
	ros::NodeHandle nh;
	ros::NodeHandle nh_param("~");
    //my_hk_control.m_nh = nh;
	get_ros_param(nh_param);
    my_hk_control.hk_init(nh);
	ros::Subscriber sub_bbox = nh.subscribe(sub_box_topic, 10, boxCallback);
	ros::Subscriber sub_joy = nh.subscribe("/joy", 10, joyCallback);
	ros::Subscriber sub_img = nh.subscribe("/hk_temperature_video", 10, imageCb);
	pub_data = nh.advertise<pdt_msgs::hk>("/hk/data",1000);
	ros::Rate loop_rate(50);
  
	while (ros::ok())
	{
		if (my_hk_control.a_manul && !my_hk_control.b_auto)
		{
			my_hk_control.hk_control_manual();
			my_hk_control.mannul_to_auto = true;
		}
		else
		{
		    my_hk_control.hk_control_auto();
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}

void get_ros_param(ros::NodeHandle &nh_param)
{	
        std::string output_file,img_path;
	int temprate_detect, sleep_time, sleep_time_detect, set_nav_position_sum;
	if(!nh_param.getParam("sub_box_topic", sub_box_topic))  
		sub_box_topic = "/pedstrian_bboxes";
	
	if(!nh_param.getParam("P", P))  P = 1;
	if(!nh_param.getParam("I", I))  I = 0;	
	if(!nh_param.getParam("D", D))  D = 0;
	
	if(!nh_param.getParam("ip", my_hk_control.ip))  my_hk_control.ip = "192.168.0.200";
	if(!nh_param.getParam("username", my_hk_control.username))  my_hk_control.username = "admin";	
	if(!nh_param.getParam("passwd", my_hk_control.passwd))  my_hk_control.passwd = "12345";
	
	if(!nh_param.getParam("keep_static_threshold", keep_static_threshold)) keep_static_threshold = 0;
	if(!nh_param.getParam("joy_sensitivity", joy_sensitivity)) joy_sensitivity = 0;
	if(!nh_param.getParam("time_threshold", time_threshold)) time_threshold = 10;
	if(!nh_param.getParam("output_file", output_file)) output_file = "/config/logs/result.txt";
	if(!nh_param.getParam("img_path", img_path)) img_path = "/config/warning.jpg";
	if(!nh_param.getParam("temprate_detect", temprate_detect)) temprate_detect = 0;
	if(!nh_param.getParam("sleep_time", sleep_time)) sleep_time = 10;
	if(!nh_param.getParam("sleep_time_detect", sleep_time_detect)) sleep_time_detect = 10;
	if(!nh_param.getParam("set_nav_position_sum", set_nav_position_sum)) set_nav_position_sum = 10;
	
	std::string pkg_path = ros::package::getPath("hk_control");
	//output_file = pkg_path + output_file;
	img_path = pkg_path + img_path;
	my_hk_control.output_file = output_file;
	my_hk_control.img_path = img_path;
	my_hk_control.temprate_detect = temprate_detect;
	my_hk_control.sleep_time = sleep_time;
	my_hk_control.sleep_time_detect = sleep_time_detect;
	my_hk_control.set_nav_position_sum = set_nav_position_sum;
}

BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
    switch(lCommand)
    {
        case COMM_THERMOMETRY_ALARM:
        {
            NET_DVR_THERMOMETRY_ALARM struThermometryAlarm = {0};
            memcpy(&struThermometryAlarm, pAlarmInfo, sizeof(NET_DVR_THERMOMETRY_ALARM));
            if (0 == struThermometryAlarm.byRuleCalibType)
            {
                hk_control::TemperatureInfo ti_tmp(struThermometryAlarm.wPresetNo, struThermometryAlarm.byRuleID, 
						   struThermometryAlarm.fCurrTemperature, struThermometryAlarm.byAlarmLevel);
                my_hk_control.hk_write_temperature(ti_tmp);
            }
        }
            break;
        default:
            std::cout << "error" << std::endl;
            break;
    }

    return TRUE;
}

