#include <QPainter>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <sys/stat.h>
#include <sys/types.h> 
#include <time.h>

#include "control_pad.h"

namespace rviz_plugins {


void ControlPad::sendStop() {
  if( ros::ok() && stop_publisher_ )
  {
    std_msgs::Int8 msg;
    msg.data = 1;
    stop_publisher_.publish(msg);
  }
}

void ControlPad::saveImg() {
  // save image
  // char base_name[256];   //图像保存
	// sprintf(base_name,"/home/zhy/catkin_ws_sliding_lane/image/%04ld.jpg",count_++);
  if (! pImg_cv_ ){
    ROS_INFO("No img received yet.");
    return ;
  }
  time_t tt = time(NULL);
  struct tm* t= localtime(&tt);
  std::string dir("/home/yuxin/"+std::to_string(t->tm_year + 1900) + "-" + std::to_string(t->tm_mon + 1) + "-"
                + std::to_string( t->tm_mday) + "_" + std::to_string(t->tm_hour) + ":"
                 + std::to_string( t->tm_min) + '/');
  mkdir(dir.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
	cv::imwrite(dir + std::to_string(img_cnt_) + ".jpg",pImg_cv_->image);
  ROS_INFO("Saved img to dir %s", dir.c_str());
  img_cnt_++;
}

void ControlPad::imgCB(const sensor_msgs::Image::ConstPtr& msg){
  //ROS_INFO_STREAM("Got img.");
  pImg_cv_ =  cv_bridge::toCvCopy(*msg, sensor_msgs::image_encodings::BGR8);

}


void ControlPad::renewSensor1Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel1(static_cast<double>(msg->data));}
void ControlPad::renewSensor2Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel2(static_cast<double>(msg->data));}
void ControlPad::renewSensor3Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel3(static_cast<double>(msg->data));}
void ControlPad::renewSensor4Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel4(static_cast<double>(msg->data));}

ControlPad::ControlPad(QWidget* parent):rviz::Panel( parent ){
    img_cnt_ = 0;
    // set topic names
    stop_topic_name_ = "/send_stop_signal";
    img_topic_name_ = "/camera/color/image_raw";
    sensor1_topic_name = "/sensor1";
    sensor2_topic_name = "/sensor2";
    sensor3_topic_name = "/sensor3";
    sensor4_topic_name = "/sensor4";

    // create top layout
    QVBoxLayout* layout = new QVBoxLayout;

    // set layout
    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget( new QLabel( "Sensor 1:" ));
    label_sensor1_data_ = new QLabel("0.00");
    layout1->addWidget(label_sensor1_data_);
    QHBoxLayout* layout2 = new QHBoxLayout;
    layout2->addWidget( new QLabel( "Sensor 2:" ));
    label_sensor2_data_ = new QLabel("0.00");
    layout2->addWidget(label_sensor2_data_);
    QHBoxLayout* layout3 = new QHBoxLayout;
    layout3->addWidget( new QLabel( "Sensor 3:" ));
    label_sensor3_data_ = new QLabel("0.00");
    layout3->addWidget(label_sensor3_data_);
    QHBoxLayout* layout4 = new QHBoxLayout;
    layout4->addWidget( new QLabel( "Sensor 4:" ));
    label_sensor4_data_ = new QLabel("0.00");
    layout4->addWidget(label_sensor4_data_);

    // add layouts to top layout 
    layout->addLayout(layout1);
    layout->addLayout(layout2);
    layout->addLayout(layout3);
    layout->addLayout(layout4);

    //create a stop button
    QHBoxLayout* layout5 = new QHBoxLayout;
    stopBtn = new QPushButton("Stop");
    saveBtn = new QPushButton("Save");
    layout5->addWidget(stopBtn);
    layout5->addWidget(saveBtn);
    //add it to layout and set layout 
    layout->addLayout(layout5);
    setLayout( layout );
    //connect Btn signal and slot
    connect( saveBtn, SIGNAL( clicked() ), this, SLOT( saveImg() ));
    connect( stopBtn, SIGNAL( clicked() ), this, SLOT( sendStop() )); 
    // create sensor subscribers and publishers
    stop_publisher_ = nh_.advertise<std_msgs::Int8>(stop_topic_name_,10);
    img_subscriber_ = nh_.subscribe<sensor_msgs::Image>(img_topic_name_, 10, boost::bind(&ControlPad::imgCB, this, _1));
    sensor1_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor1_topic_name, 10, boost::bind(&ControlPad::renewSensor1Data, this, _1));
    sensor2_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor2_topic_name, 10, boost::bind(&ControlPad::renewSensor2Data, this, _1));
    sensor3_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor3_topic_name, 10, boost::bind(&ControlPad::renewSensor3Data, this, _1));
    sensor4_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor4_topic_name, 10, boost::bind(&ControlPad::renewSensor4Data, this, _1));
    // //create a Timer for test
    // QTimer* output_timer = new QTimer( this );
    // output_timer->start( 100 );
    // //connect Timer signal and slot
    // connect( output_timer, SIGNAL( timeout() ), this, SLOT( sendVel() ));
    ros::spinOnce();

    

}

void ControlPad::Setlabel1(double s) {label_sensor1_data_->setNum(s); }
void ControlPad::Setlabel2(double s) {label_sensor2_data_->setNum(s); }
void ControlPad::Setlabel3(double s) {label_sensor3_data_->setNum(s); }
void ControlPad::Setlabel4(double s) {label_sensor4_data_->setNum(s); }


}
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(rviz_plugins::ControlPad,rviz::Panel )