cmake_minimum_required(VERSION 3.0.2)
project(rviz_plugins)
set(CMAKE_CXX_FLAGS "-Wl,--no-undefined")

set(OpenCV_DIR /usr/share/OpenCV/) # set this dir!!!!!!!!!!!!!!
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rviz
  std_msgs
  sensor_msgs
  OpenCV
  cv_bridge
)

set(QT_LIBRARIES Qt5::Widgets)
find_package(Qt5Widgets)
set (Qt5QtCore_DIR /lib/x86_64-linux-gnu/cmake/Qt5Core)
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

#include(${QT_USE_FILE})
add_definitions(-DQT_NO_KEYWORDS)
qt5_wrap_cpp(MOC_FILES
  src/control_pad.h
)
set(SOURCE_FILES
  src/control_pad.cpp 
  ${MOC_FILES}
)

include_directories(${catkin_INCLUDE_DIRS}) 
include_directories(${PROJECT_SOURCE_DIR})
# link_directories(${catkin_LIBRARY_DIRS}) 
add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets) 
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${catkin_LIBRARIES})

