#ifndef CONTROL_PAD_H
#define CONTROL_PAD_H

#include <ros/ros.h>
#include <ros/console.h>
#include <rviz/panel.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Image.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "image_transport/image_transport.h"
#include "opencv2/highgui/highgui.hpp"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.h"

class QString;
class QPushButton;
class QLabel;

namespace rviz_plugins {
    class ControlPad:public rviz::Panel{
        Q_OBJECT
        public:
        ControlPad( QWidget* parent = 0 );
       
        //公共槽.
        public Q_SLOTS:

        // 内部槽.
        protected Q_SLOTS:
        void sendStop();                    // send stop
        void saveImg();                     // save picture
        void imgCB(const sensor_msgs::Image::ConstPtr& img);
        void renewSensor1Data(const std_msgs::Float32::ConstPtr& msg);        // renew sensor data
        void renewSensor2Data(const std_msgs::Float32::ConstPtr& msg);        // renew sensor data
        void renewSensor3Data(const std_msgs::Float32::ConstPtr& msg);        // renew sensor data
        void renewSensor4Data(const std_msgs::Float32::ConstPtr& msg);        // renew sensor data
        public:
        void Setlabel1(double s);
        void Setlabel2(double s);
        void Setlabel3(double s);
        void Setlabel4(double s);
        // 内部变量.
        public:
        // Qt widgets
        std::string stop_topic_name_;
        std::string img_topic_name_;
        std::string sensor1_topic_name;
        std::string sensor2_topic_name;
        std::string sensor3_topic_name;
        std::string sensor4_topic_name;
        QPushButton* stopBtn;
        QPushButton* saveBtn;
        QLabel* label_sensor1_data_;
        QLabel* label_sensor2_data_;
        QLabel* label_sensor3_data_;
        QLabel* label_sensor4_data_;

        protected:
        // The ROS node handle.
        ros::NodeHandle nh_;

        // ROS publishers and subscribers
        ros::Publisher stop_publisher_;
        ros::Subscriber img_subscriber_;
        ros::Subscriber sensor1_subscriber_;
        ros::Subscriber sensor2_subscriber_;
        ros::Subscriber sensor3_subscriber_;
        ros::Subscriber sensor4_subscriber_;
        
        // private data
        cv_bridge::CvImagePtr pImg_cv_;
        int img_cnt_;
    };
}

#endif