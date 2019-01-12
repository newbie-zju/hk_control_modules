#pragma once

#include "hk_control_common.h"

namespace hk_control{

class TemperatureInfo
{
public:
    int num;
    int id;
    float temperature;
    int key;
    int level;
    
    TemperatureInfo(){}

    TemperatureInfo(int wPresetNo, int byRuleID, float fCurrTemperature, int byAlarmLevel)
    {
        num = wPresetNo;
        id = byRuleID;
        temperature = fCurrTemperature;
        key = num + 100*id;
	level = byAlarmLevel;
    }

};

class Hk_Control
{
  
public:
	Hk_Control();
	~Hk_Control();
	
	void hk_init(ros::NodeHandle &m_nh);
	void hk_control_process(float target_center_x, float target_center_y, std_msgs::Header detect_head);
	void hk_control_manual();
	void hk_control_auto();
	void hk_write_temperature(TemperatureInfo ti);
	
	inline std::string getTime()
	{
	    time_t timep;
	    time (&timep);
	    char tmp[64];
	    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
	    return tmp;
	}

private:
	void hk_control_span(float x_differ, float y_differ,float  control_threshold);
// 	void hk_control_auto(float target_center_x, float target_center_y, std_msgs::Header detect_head);
	void hk_control_reset();
	void loadParams();
	void hk_control_speed(bool is_stop, int rotation_speed);
	void hk_control_navgation();
	void imageCb(const sensor_msgs::ImageConstPtr &msg);
	static void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
// 	static BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
	inline void hk_clean()
	{
	    ti_vector.clear();
	}

	
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

	LONG lUserID, lHandle;;
	std::string ip, username, passwd;
	int a_manul, b_auto;
	double ax, ay, az;
	std::string output_file, img_path;
	bool mannul_to_auto;
	int temprate_detect, sleep_time, sleep_time_detect, set_nav_position_sum;
	std::vector<TemperatureInfo> ti_vector;
	cv::Mat src_img;
	
private:
	std_msgs::Header count_time;
	float delta_x, delta_y;
	bool has_launch, has_set_position, not_first_undetect, warn_show;
	bool img_show;
	HKActionEnum hk_action_state, last_hk_action_state;
	ros::Time start_time, set_position_time;
	int nav_position_num;
	int preset_position_sum, ID_sum;
	std::vector<std::string> show_position;
	ros::Publisher m_pubstate;
};

}
