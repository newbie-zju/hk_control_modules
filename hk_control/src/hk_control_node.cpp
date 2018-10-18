#include "hk_control.h"


float P, I, D;
float keep_static_threshold, joy_sensitivity;
int time_threshold;
std::string sub_box_topic;

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
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "svm_train_node");
	ros::NodeHandle nh;
	ros::NodeHandle nh_param("~");
	get_ros_param(nh_param);
	
	ros::Subscriber sub_bbox = nh.subscribe(sub_box_topic, 10, boxCallback);
	ros::Subscriber sub_joy = nh.subscribe("/joy", 10, joyCallback);
	ros::Rate loop_rate(50);
  
	while (ros::ok())
	{
		if (my_hk_control.a_manul && !my_hk_control.b_auto)
		{
			my_hk_control.hk_control_manual();
// 			std::cout << "manul mode" << std::endl;
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}

void get_ros_param(ros::NodeHandle &nh_param)
{
	if(!nh_param.getParam("sub_box_topic", sub_box_topic))  
		sub_box_topic = "/pedstrian_bboxes";
	
	if(!nh_param.getParam("P", P))  P = 1;
	if(!nh_param.getParam("I", I))  I = 0;	
	if(!nh_param.getParam("D", D))  D = 0;
	
	if(!nh_param.getParam("ip", my_hk_control.ip))  my_hk_control.ip = "192.0.0.64";
	if(!nh_param.getParam("username", my_hk_control.username))  my_hk_control.username = "admin";	
	if(!nh_param.getParam("passwd", my_hk_control.passwd))  my_hk_control.passwd = "12345";
	
	if(!nh_param.getParam("keep_static_threshold", keep_static_threshold)) keep_static_threshold = 0;
	if(!nh_param.getParam("joy_sensitivity", joy_sensitivity)) joy_sensitivity = 0;
	if(!nh_param.getParam("time_threshold", time_threshold)) time_threshold = 10;
	std::string pkg_path = ros::package::getPath("hk_control");
	
	my_hk_control.hk_init();
}
