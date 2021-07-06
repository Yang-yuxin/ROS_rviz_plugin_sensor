#ifndef CONTROL_PAD_H
#define CONTROL_PAD_H

#include <ros/ros.h>
#include <ros/console.h>
#include <rviz/panel.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32.h>
#include <string>

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

        // The ROS node handle.
        ros::NodeHandle nh_;

        // ROS publishers and subscribers
        ros::Publisher stop_publisher_;
        ros::Subscriber sensor1_subscriber_;
        ros::Subscriber sensor2_subscriber_;
        ros::Subscriber sensor3_subscriber_;
        ros::Subscriber sensor4_subscriber_;
        
        // private data
        float sensor1_data_;
        float sensor2_data_;
        float sensor3_data_;
        float sensor4_data_;        

    };
}

#endif