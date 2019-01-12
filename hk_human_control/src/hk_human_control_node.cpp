#include "hk_human_control.h"


float P, I, D;
float keep_static_threshold, joy_sensitivity;
int time_threshold;
std::string sub_box_topic;
ros::Publisher pub_data;

hk_human_control::Hk_Control my_hk_control;

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

	my_hk_control.hk_control_auto(target_center_x, target_center_y, detect_timestamp);
	//my_hk_control.hk_control_process();
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "svm_train_node");
	ros::NodeHandle nh;
	ros::NodeHandle nh_param("~");

	get_ros_param(nh_param);
    my_hk_control.hk_init(nh);
	ros::Subscriber sub_bbox = nh.subscribe(sub_box_topic, 10, boxCallback);
	//ros::Subscriber sub_joy = nh.subscribe("/joy", 10, joyCallback);
	ros::Rate loop_rate(50);
  
	while (ros::ok())
	{
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

