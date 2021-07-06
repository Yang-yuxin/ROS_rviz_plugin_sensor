#include <QPainter>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QWidget>


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
  
}

void ControlPad::renewSensor1Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel1(static_cast<double>(msg->data));}
void ControlPad::renewSensor2Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel2(static_cast<double>(msg->data));}
void ControlPad::renewSensor3Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel3(static_cast<double>(msg->data));}
void ControlPad::renewSensor4Data(const std_msgs::Float32::ConstPtr& msg) { Setlabel4(static_cast<double>(msg->data));}

ControlPad::ControlPad(QWidget* parent):rviz::Panel( parent ){
    
    // set topic names
    stop_topic_name_ = "/send_stop_signal";
    sensor1_topic_name = "/sensor1";
    sensor2_topic_name = "/sensor2";
    sensor3_topic_name = "/sensor3";
    sensor4_topic_name = "/sensor4";

    // set default sensor values
    sensor1_data_ = 0;
    sensor2_data_ = 0;
    sensor3_data_ = 0;
    sensor4_data_ = 0; 
    // create top layout
    QVBoxLayout* layout = new QVBoxLayout;

    // set layout
    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget( new QLabel( "Sensor 1:" ));
    label_sensor1_data_ = new QLabel;
    label_sensor1_data_->setNum(0);
    layout1->addWidget(label_sensor1_data_);
    QHBoxLayout* layout2 = new QHBoxLayout;
    layout2->addWidget( new QLabel( "Sensor 2:" ));
    label_sensor2_data_ = new QLabel(QString::number(sensor2_data_));
    layout2->addWidget(label_sensor2_data_);
    QHBoxLayout* layout3 = new QHBoxLayout;
    layout3->addWidget( new QLabel( "Sensor 3:" ));
    label_sensor3_data_ = new QLabel(QString::number(sensor3_data_));
    layout3->addWidget(label_sensor3_data_);
    QHBoxLayout* layout4 = new QHBoxLayout;
    layout4->addWidget( new QLabel( "Sensor 4:" ));
    label_sensor4_data_ = new QLabel(QString::number(sensor4_data_));
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
    layout->addlayout(layout5);
    setLayout( layout );
    //connect Btn signal and slot
    connect( saveBtn, SIGNAL( clicked() ), this, SLOT( saveImg() ));
    connect( stopBtn, SIGNAL( clicked() ), this, SLOT( sendStop() )); 
    // create sensor subscribers and publishers
    stop_publisher_ = nh_.advertise<std_msgs::Int8>(stop_topic_name_,10);
    sensor1_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor1_topic_name, 10, boost::bind(&ControlPad::renewSensor1Data, this, _1));
    sensor2_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor2_topic_name, 10, boost::bind(&ControlPad::renewSensor2Data, this, _1));
    sensor3_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor3_topic_name, 10, boost::bind(&ControlPad::renewSensor3Data, this, _1));
    sensor4_subscriber_ =  nh_.subscribe<std_msgs::Float32>(sensor4_topic_name, 10, boost::bind(&ControlPad::renewSensor4Data, this, _1));
    // //create a Timer for test
    // QTimer* output_timer = new QTimer( this );
    // output_timer->start( 100 );
    // //connect Timer signal and slot
    // connect( output_timer, SIGNAL( timeout() ), this, SLOT( sendVel() ));
    

    

}

void ControlPad::Setlabel1(double s) {label_sensor1_data_->setNum(s); }
void ControlPad::Setlabel2(double s) {label_sensor2_data_->setNum(s); }
void ControlPad::Setlabel3(double s) {label_sensor3_data_->setNum(s); }
void ControlPad::Setlabel4(double s) {label_sensor4_data_->setNum(s); }


}
#include <pluginlib/class_list_macros.h>

PLUGINLIB_EXPORT_CLASS(rviz_plugins::ControlPad,rviz::Panel )