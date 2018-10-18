#include "hk_control.h"

namespace hk_control {
  
Hk_Control::Hk_Control():has_launch(false), ax(0), ay(0), a_manul(0), b_auto(1), 
has_set_position(false), not_first_undetect(false)
{

}

Hk_Control::~Hk_Control()
{
	ROS_INFO("Destroy the windows");
}

void Hk_Control::hk_control_process(float target_center_x, float target_center_y, std_msgs::Header detect_head)
{	
// 	if (a_manul && !b_auto)
// 		hk_control_manual();
	if (!a_manul && b_auto)
	{
		hk_control_auto(target_center_x, target_center_y, detect_head);
// 		std::cout << "auto mode" << std::endl;
	}
}

void Hk_Control::hk_control_auto(float target_center_x, float target_center_y, std_msgs::Header detect_head)
{
	if (!not_first_undetect)
	{
		count_time = detect_head;
		not_first_undetect = true;
	}
	
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
		
	}
  
}

void Hk_Control::hk_control_manual()
{

	double joy_x, joy_y;

	joy_x = ax;
	joy_y = ay;
	
	hk_control_span(joy_x, joy_y, joy_sensitivity);
	
	if (has_set_position)
	{
		if (!NET_DVR_PTZPreset_Other(lUserID, 1, 9, 2))
		{
			printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
			NET_DVR_Cleanup();
			return;
		}
		has_set_position = false;
	}
	
	if (!NET_DVR_PTZPreset_Other(lUserID, 1, 8, 2))
	{
		printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}
	else
	{
		has_set_position = true;
	}	

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
// 		std::cout << "control_state[index]" << control_state[index] << std::endl;
		if(!NET_DVR_PTZControlWithSpeed_Other(lUserID, 1, hk_action_state, 0, rotation_speed))
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


void Hk_Control::hk_init()
{
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

	// turn on for setting the preset position
	if (!NET_DVR_PTZPreset_Other(lUserID, 1, 8, 1))
	{
		printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}
}

void Hk_Control::hk_control_reset()
{

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
	
}