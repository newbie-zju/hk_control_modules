#pragma once

/**************************************/
/*            ros head file          */
/**************************************/
#include "ros/ros.h"
#include <sensor_msgs/Joy.h>
// #include <cv_bridge/cv_bridge.h>
#include <ros/package.h>
#include "pdt_msgs/BoundingBox.h"

/**************************************/
/*            hk head file          */
/**************************************/
#include "HCNetSDK.h"

/**************************************/
/*      opencv/eigen head  file       */
/**************************************/
//
#include <opencv2/opencv.hpp>

/**************************************/
/*             Macro define           */
/**************************************/
#define img_x 0.5
#define img_y 0.5

/**************************************/
/*       head function define         */
/**************************************/
extern float P, I, D;
extern float keep_static_threshold, joy_sensitivity;
extern int time_threshold;
