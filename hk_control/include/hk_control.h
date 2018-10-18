#pragma once

#include "hk_control_common.h"

namespace hk_control{
    
class Hk_Control
{
public:
	Hk_Control();
	~Hk_Control();
	
	void hk_init();
	void hk_control_process(float target_center_x, float target_center_y, std_msgs::Header detect_head);
	void hk_control_manual();

private:
	void hk_control_span(float x_differ, float y_differ,float  control_threshold);
	void hk_control_auto(float target_center_x, float target_center_y, std_msgs::Header detect_head);
	void hk_control_reset();
	void hk_control_speed(bool is_stop, int rotation_speed);
	static void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);

	
public:
	enum HKActionEnum
	{
		HK_UP=21,
		HK_DOWN,
		HK_LEFT,
		HK_RIGHT,
		HK_UP_LEFT,
		HK_UP_RIGHT,
		HK_DOWN_LEFT,
		HK_DOWN_RIGHT,
	};

	LONG lUserID;
	std::string ip, username, passwd;
	int a_manul, b_auto;
	double ax, ay;
	
private:
	std_msgs::Header count_time;
	float delta_x, delta_y;
	bool has_launch, has_set_position, not_first_undetect;
	HKActionEnum hk_action_state, last_hk_action_state;
};

}
