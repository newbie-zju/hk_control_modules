#include "hk_control.h"

namespace hk_control {
  
Hk_Control::Hk_Control():has_launch(false), ax(0), ay(0), a_manul(0), b_auto(1), 
has_set_position(false), not_first_undetect(false), mannul_to_auto(false), temprate_detect(false),
nav_position_num(2), warn_show(false), img_show(false)
{
  
}

Hk_Control::~Hk_Control()
{
    ROS_INFO("Destroy the windows");

    //---------------------------------------
    if (!NET_DVR_CloseAlarmChan_V30(lHandle))
    {
        printf("NET_DVR_CloseAlarmChan_V30 error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
    }
    else
    {
	//---------------------------------------
	NET_DVR_Logout(lUserID);

	//---------------------------------------
	NET_DVR_Cleanup();
    }

}

void Hk_Control::hk_control_process(float target_center_x, float target_center_y, std_msgs::Header detect_head)
{	
// 	if (a_manul && !b_auto)
// 		hk_control_manual();
	if (!a_manul && b_auto)
	{
// 		hk_control_auto(target_center_x, target_center_y, detect_head);
// 		std::cout << "auto mode" << std::endl;
	}
}

// void Hk_Control::hk_control_auto(float target_center_x, float target_center_y, std_msgs::Header detect_head)
// {
// 	if (!not_first_undetect)
// 	{
// 		count_time = detect_head;
// 		not_first_undetect = true;
// 	}
// 	
// 	if (mannul_to_auto)
// 	{
// 	     start_time = ros::Time::now();
// 	     mannul_to_auto = false;
// 	}
// 	
// 	if (temprate_detect)
// 	{
// 	    hk_control_navgation();
// 	    return;
// 	}
// 	
// 	double undetect_time;
// 	undetect_time = detect_head.stamp.toSec() - count_time.stamp.toSec();
// 	
// 	if (target_center_x == -1)
// 	{
// 		if (undetect_time > time_threshold)
// 		{
// 			if (!NET_DVR_PTZPreset_Other(lUserID, 1, 39, 2))
// 			{
// 				printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
// 				NET_DVR_Cleanup();
// 				return;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		delta_x = P * (img_x - target_center_x);
// 		delta_y = P * (img_y - target_center_y);
// 		count_time = detect_head;
// 		
// 		hk_control_span(delta_x, delta_y, keep_static_threshold);
// 		
// 	}
//   
// }

void Hk_Control::hk_control_auto()
{
	if (!not_first_undetect)
	{
// 		count_time = detect_head;
		not_first_undetect = true;
	}
	set_position_time = ros::Time::now();
	if (mannul_to_auto)
	{
	     start_time = ros::Time::now();
	     mannul_to_auto = false;
	}
	
	if (temprate_detect)
	{
	    hk_control_navgation();
	    return;
	}
/*	
	double undetect_time;
	undetect_time = detect_head.stamp.toSec() - count_time.stamp.toSec();
	
	if (target_center_x == -1)
	{
		if (undetect_time > time_threshold)
		{
			if (!NET_DVR_PTZPreset_Other(lUserID, 1, 39, 2))
			{
				printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
				NET_DVR_Cleanup();
				return;
			}
		}
	}
	else
	{
		delta_x = P * (img_x - target_center_x);
		delta_y = P * (img_y - target_center_y);
		count_time = detect_head;
		
		hk_control_span(delta_x, delta_y, keep_static_threshold);
		
	}*/
  
}


void Hk_Control::hk_control_manual()
{

	double joy_x, joy_y;

	joy_x = ax;
	joy_y = ay;
	
	hk_control_span(joy_x, joy_y, joy_sensitivity);
	
// 	double t_set_position;
// 	t_set_position = (ros::Time::now() - set_position_time).toSec();
// 	if (t_set_position < 1.0)
// 	{
// 	   az = 0;
// 	}
// 	
// 	if (az == 1)
// 	{
// 		if (!NET_DVR_PTZPreset_Other(lUserID, 1, 8, nav_position_num))
// 		{
// 			printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
// 			NET_DVR_Cleanup();
// 			return;
// 		}
// 		set_position_time = ros::Time::now();
// 		printf("set the nav position: %d\n", nav_position_num - 1);
// 		nav_position_num = nav_position_num + 1;
// 		set_nav_position_sum = set_nav_position_sum + 1;
// 		az = 0;
// 	}

}

void Hk_Control::hk_control_navgation()
{
    std_msgs::Bool change_mode_msgs;
	change_mode_msgs.data = false;
    if (set_nav_position_sum == 0 && !warn_show)
    {
	printf("set the nav position first\n");
	warn_show = true;
	return;
    }
    hk_clean();
    for (size_t i = 1; i <= set_nav_position_sum; i++)
    {
//         hk_clean();
		if (i == 6)
		{
			change_mode_msgs.data = true;
		}
		
		if (!NET_DVR_PTZPreset_Other(lUserID, 1, 39, i))
		{
			printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
			NET_DVR_Cleanup();
			return;
		}
		sleep(3);
		m_pubstate.publish(change_mode_msgs);
		if (!ros::ok()) 
		{
			printf("quit for navigation\n");
			break;
		}
		printf("back to set position: %d\n", i);
		if (i != 6)  sleep(sleep_time);
    }
    printf("\n");
    sleep(sleep_time_detect);

    change_mode_msgs.data = false;
    m_pubstate.publish(change_mode_msgs);
	
}


void Hk_Control::hk_control_speed(bool is_stop, int rotation_speed)
{
	if (is_stop)
	{
		if (!has_launch)  return;
		// stop turn
		if(!NET_DVR_PTZControlWithSpeed_Other(lUserID, 1, last_hk_action_state, 1, 1))
		{
			printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
			NET_DVR_Cleanup();
			return;
		}
		has_launch = false;
// 		printf("stop!\n");
	}
	else
	{
		if (has_launch)
		{
			if(!NET_DVR_PTZControlWithSpeed_Other(lUserID, 1, last_hk_action_state, 1, 1))
			{
				printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
				NET_DVR_Cleanup();
				return;
			}
			has_launch = false;
// 			printf("stop!\n");
		}
// 		printf("hk_action_state: %d\n", hk_action_state);
// 		std::cout << "control_state[index]" << control_state[index] << std::endl;
		if(!NET_DVR_PTZControlWithSpeed_Other(lUserID, 2, hk_action_state, 0, rotation_speed))
		{
			printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
			NET_DVR_Cleanup();
			return;
		}
		has_launch = true;
// 		printf("start to turn !%d\n", index);
	}
	

}

void Hk_Control::hk_control_span(float x_differ, float y_differ,float  control_threshold)
{
  	double angle_degree, angle_radian, max_button;
	int control_speed;
	bool is_stop;
	
	max_button = std::max(fabs(x_differ), fabs(y_differ));
	control_speed = std::floor(max_button*7);
	if (control_speed == 0)  control_speed = 1;
	angle_radian = atan2(x_differ, y_differ);
	angle_degree = angle_radian * (180 / M_PI);
// 	std::cout << "x_differ" << x_differ << std::endl;
// 	std::cout << "y_differ" << y_differ << std::endl;
	if (!(fabs(x_differ) < control_threshold && fabs(y_differ) < control_threshold))
	{
		is_stop = false;
	  	// up
		if (angle_degree > -30 && angle_degree <= 30) hk_action_state = HK_UP;
		// down
		else if (angle_degree > 150 || angle_degree <= -150) hk_action_state = HK_DOWN;
		// left
		else if (angle_degree > 60 && angle_degree <= 120) hk_action_state = HK_LEFT;
		// right
		else if (angle_degree > -120 && angle_degree <= -60) hk_action_state = HK_RIGHT;
		// up left 
		else if (angle_degree > 30 && angle_degree <= 60) hk_action_state = HK_UP_LEFT;
		// up right
		else if (angle_degree > -60 && angle_degree <= -30) hk_action_state = HK_UP_RIGHT;
		// down left
		else if (angle_degree > 120 && angle_degree <= 150) hk_action_state = HK_DOWN_LEFT;
		// down right
		else if (angle_degree > -150 && angle_degree <= -120) hk_action_state = HK_DOWN_RIGHT;
// 		std::cout << "hk_state" << hk_action_state << std::endl;
		last_hk_action_state = hk_action_state;
	}
	else
	{
		is_stop = true;
	}
	
	hk_control_speed(is_stop, control_speed);
}


void Hk_Control::hk_init(ros::NodeHandle &m_nh)
{
	loadParams();
	m_pubstate = m_nh.advertise<std_msgs::Bool>("/hk_mode", 10);
	NET_DVR_Init();

	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
	strcpy((char *)struLoginInfo.sDeviceAddress,  (char *)ip.data());
	strcpy((char *)struLoginInfo.sUserName, (char *) username.data());
	strcpy((char *)struLoginInfo.sPassword,  (char *)passwd.data());

	struLoginInfo.wPort = 8000;
	struLoginInfo.bUseAsynLogin = 0;
	lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
	
	if (lUserID < 0)  throw std::string("hk state abnormal");

	NET_DVR_SetDVRMessageCallBack_V31(MessageCallback, NULL);
	
	NET_DVR_SETUPALARM_PARAM  struAlarmParam={0};
	struAlarmParam.dwSize=sizeof(struAlarmParam);

	lHandle = NET_DVR_SetupAlarmChan_V41(lUserID, & struAlarmParam);
	if (lHandle < 0)
	{
	    printf("NET_DVR_SetupAlarmChan_V41 error, %d\n", NET_DVR_GetLastError());
	    NET_DVR_Logout(lUserID);
	    NET_DVR_Cleanup();
	    return;
	}

	// turn on for setting the preset position　６
	if (!NET_DVR_PTZPreset_Other(lUserID, 1, 39, 6))
	{
		printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}
	sleep(10);
	
}

void Hk_Control::hk_write_temperature(TemperatureInfo ti)
{        
    for (int i = 0; i < ti_vector.size() ; i++)
        if (ti_vector[i].key == ti.key)
            return;

        std::ifstream ifile;
        ifile.open(output_file);
        if(!ifile)
        {
            std::cout << "cannot find: " << output_file << std::endl;
            std::ofstream ofile;
            ofile.open(output_file);
            ofile << std::endl;
            ofile.close();
            ifile.open(output_file);
            if(!ifile)
            {
                std::cout << "failed" << std::endl;
                return;
            }
            else
            {
                std::cout << "sucess" << std::endl;
                ifile.close();
            }
        }
        std::string position_name_str;
        int position_name_str_index;
        position_name_str_index = ID_sum*(ti.num - 1) + ti.id - 1;
        if (position_name_str_index > show_position.size())
        {
            printf("error ID and pre_position\n");
	        position_name_str = "error ID and pre_position";
        }
         else
        {
            position_name_str = show_position[position_name_str_index];
        }
        std::ofstream ofile;
        ofile.open(output_file, std::ios::app);
        ofile<<"时间:" << getTime() << ", " <<"预置点:" << ti.num << ", " << "ID:"<< ti.id << ", " << "位置: " << position_name_str << ", " << "温度:"<< ti.temperature << std::endl;
	    printf("record the temerature\n");
        std::cout<<"时间:" << getTime() << ", " <<"预置点:" << ti.num << ", " << "ID:"<< ti.id << ", " << "温度:"<< ti.temperature << std::endl;

	if (ti.level == 1) 
	{
            if (src_img.type() != 0) 
	    {
	        cv::Mat src_org;
                std::string image_output_file = output_file;
                image_output_file = image_output_file.replace(image_output_file.find(".txt"), 4, + " time:"+
                                                              getTime() + " num:" + std::to_string(ti.num) + " id:" +
                                                              std::to_string(ti.id) + " t:" + std::to_string(ti.temperature) +
                                                              " l:" + std::to_string(ti.level) + ".jpg");
                std::cout << image_output_file << std::endl;
                cv::imwrite(image_output_file, src_img);
                src_img = src_org;//clean
            } 
            else 
	    {
                std::cout << "未收到红外线图像" << std::endl;
            }

        }

	pdt_msgs::hk hk_data;
	hk_data.data_time = getTime();
	hk_data.pre_position = std::to_string(ti.num);
	hk_data.ID = std::to_string(ti.id);
	hk_data.temperature = std::to_string(ti.temperature);
	pub_data.publish(hk_data);
	
    ti_vector.push_back(ti);
}

void CALLBACK Hk_Control::g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = {0};
	switch(dwType)
	{
		case EXCEPTION_RECONNECT:
			printf("----------reconnect--------%d\n", time(NULL));
			break;
		default:
			break;
	}
}

void Hk_Control::loadParams()
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

}