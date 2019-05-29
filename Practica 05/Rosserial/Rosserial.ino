#include<ros.h>
#include<std_msgs/Float32.h>

ros::NodeHandle nh;

std_msgs::Float32 sharpSensor;
std_msgs::Float32 sharpSensor2;

ros::Publisher sharpSensorPub("/sharp_sensor",&sharpSensor);
ros::Publisher sharpSensorPub2("/sharp_sensor2",&sharpSensor2);

void speedMotorCallback(const std std_msgs::Float32& mess){
	digitalWrite(13,HIGH);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  nh.initNode();
  nh.advertise(sharpSensorPub);
  nh.advertise(sharpSensorPub2);
}

void loop() {
  // put your main code here, to run repeatedly:
  sharpSensor.data=digitalRead(7);//0.5; //Envía el dato a ROS
  sharpSensorPub.publish(&sharpSensor);
  sharpSensor2.data=digitalRead(8);//0.5; //Envía el dato a ROS
  sharpSensorPub2.publish(&sharpSensor2);
  nh.spinOnce();
  delay(500);
}
