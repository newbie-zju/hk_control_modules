cmake_minimum_required(VERSION 2.8.3)
project(main_window)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Release)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  cv_bridge
  sensor_msgs
  roslib
  pdt_msgs
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
qt5_wrap_cpp(MOC include/mainwindow.h include/rosthread.h)
qt5_wrap_ui(UIC config/ui/mainwindow.ui)


catkin_package(

)


include_directories(
  include
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(${PROJECT_NAME}_node 
  src/main.cpp 
  src/mainwindow.cpp 
  src/rosthread.cpp
  include/mainwindow.h
  include/rosthread.h
  config/ui/mainwindow.ui
)

add_dependencies(${PROJECT_NAME}_node 
  pdt_msgs_generate_messages_cpp
  )
  
target_link_libraries(${PROJECT_NAME}_node 
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
  Qt5::Widgets
  Qt5::Core 
  Qt5::Gui
)

