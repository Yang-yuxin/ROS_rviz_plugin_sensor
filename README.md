# ROS_rviz_plugin_sensor

## Overview

**A plugin for rviz** using ROS platform. The plugin integrates data of a few sensors, and is also able to subscribe certain image topic to display real-time capture.

## Requirements

* Ubuntu 18.04
* ROS melodic installed
* opencv 3.x.x (I'm using 3.2.0, I assume version doesn't matter though)
* Qt5 - use `sudo apt-get install qt5-default` to install qt5 as default qt.

## Download, modify, build

### Download

 Open terminal and enter the following commands ( be sure to have a workspace initiated ) : 

```bash
cd catkin_ws/src
git clone --recursive https://github.com/Yang-yuxin/ROS_rviz_plugin_sensor.git
```

### Specify paths 

Specify your own path to dependency in CMakeLists.txt : 

```cmake
set (OpenCV_DIR /usr/share/OpenCV/) # set this dir to your OpenCV path
set (Qt5QtCore_DIR /lib/x86_64-linux-gnu/cmake/Qt5Core) # set your Qt5QtCore dir
```

### Specify topics

Specify your topics :

Specify the topics to your own topics in `src/control_pad.cpp`

The plugin contains a publisher of a stop_topic (initially designed to send a stop signal)  named /send_stop_signal, and five subscribers. You can specify the sensor topics to your own. 

```c++
stop_topic_name_ = "/send_stop_signal";
img_topic_name_ = "/darknet_ros/detection_image"; // the topic name is specified as I'm using it to receive object detection stream from darknet_ros.
sensor1_topic_name = "/sensor1";
sensor2_topic_name = "/sensor2";
sensor3_topic_name = "/sensor3";
sensor4_topic_name = "/sensor4";
```

### Build

Return to the root of the workspace and build the project:

```bash
cd ..
catkin_make
```

Finally, don't forget to run `source devel/setup.bash` to add the plugin in rviz.

## Usage

Start roscore, run rviz and select "Panels- Add New Panal", you should be seeing a folder named "rviz_plugins" and a plugin named "CtrlPad". Click on it and start using!

## Others 

This is literally my first trial to build a ros project on my own, I'd be glad if you could inform me on finding anything unclear or weird. Thank you!

