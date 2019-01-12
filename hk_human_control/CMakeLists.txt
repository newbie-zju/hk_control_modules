cmake_minimum_required(VERSION 2.8.3)
project(hk_human_control)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Release)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
  sensor_msgs
  std_msgs
  pdt_msgs
  cv_bridge
)

find_package(OpenCV REQUIRED)

catkin_package(
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node 
  src/hk_human_control_node.cpp 
  src/hk_human_control.cpp
)

file(GLOB hk_SO_FILES
  ${PROJECT_SOURCE_DIR}/lib/*.so
  ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/*.so
  
)

add_dependencies(${PROJECT_NAME}_node 
  pdt_msgs_generate_messages_cpp
  )

target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
  ${hk_SO_FILES}
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libanalyzedata.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCAlarm.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCCoreDevCfg.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCDisplay.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCGeneralCfgMgr.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCIndustry.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCPlayBack.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCPreview.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libHCVoiceTalk.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libiconv2.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libStreamTransClient.so
#   ${PROJECT_SOURCE_DIR}/lib/HCNetSDKCom/libSystemTransform.so
#   ${PROJECT_SOURCE_DIR}/lib/libAudioRender.so
#   ${PROJECT_SOURCE_DIR}/lib/libHCCore.so
#   ${PROJECT_SOURCE_DIR}/lib/libhcnetsdk.so
#   ${PROJECT_SOURCE_DIR}/lib/libhpr.so
#   ${PROJECT_SOURCE_DIR}/lib/libPlayCtrl.so
#   ${PROJECT_SOURCE_DIR}/lib/libSuperRender.so
)
