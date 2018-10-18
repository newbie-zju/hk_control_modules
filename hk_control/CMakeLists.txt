cmake_minimum_required(VERSION 2.8.3)
project(hk_control)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Release)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
  sensor_msgs
  std_msgs
  pdt_msgs
)

catkin_package(
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node 
  src/hk_control_node.cpp 
  src/hk_control.cpp
)

add_dependencies(${PROJECT_NAME}_node pdt_msgs_generate_messages_cpp)

target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${PROJECT_SOURCE_DIR}/lib/libAudioRender.so
  ${PROJECT_SOURCE_DIR}/lib/libHCCore.so
  ${PROJECT_SOURCE_DIR}/lib/libhcnetsdk.so
  ${PROJECT_SOURCE_DIR}/lib/libhpr.so
  ${PROJECT_SOURCE_DIR}/lib/libPlayCtrl.so
  ${PROJECT_SOURCE_DIR}/lib/libSuperRender.so
)
