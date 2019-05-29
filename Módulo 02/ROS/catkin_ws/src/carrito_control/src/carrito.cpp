#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int64.h>
#include <chrono>
#include <thread>
#define v_max 0.5
/*void subEncoderCallback(const std_msgs::int64::ConstPtr &msg){
	std::cout << msg->data << std::endl;
}*/


ros::Publisher pubVelMotor1 = nh.advertise<std_msgs::Float32>("/speed_motor_1",1);
ros::Publisher pubVelMotor2 = nh.advertise<std_msgs::Float32>("/speed_motor_2",1);

std_msgs::Float32 msgI;
std_msgs::Float32 msgD;
	
float avanza(float distancia){	
	
	ros::Time start_time=ros::Time::now();

	float tf=(1080*(distancia))/(8.4*3.1416*v_max);

	while(1){

		if(delta<t1){
			msgI.data =v_max*delta/t1;
			msgD.data =v_max*delta/t1;
		}
		if(delta>=t1 && delta<t2){
			msgI.data=v_max;
			msgD.data=v_max;
		}
		if(delta>=t2 && delta<=t3){
			msgI.data=(v_max-((v_max*(delta-t2))/(t3-t2)));
			msgD.data=(v_max-((v_max*(delta-t2))/(t3-t2)));
		}
		if(delta>t3){
			msgI.data=0;
			msgD.data=0;
			break;
		}
		pubVelMotor1.publish(msgI);
		pubVelMotor2.publish(msgD);

		ros::spinOnce();
		rate.sleep();

		ros::Duration delta_t= ros::Time::now() - start_time;
		delta= (delta_t.toSec())*1000;

	}

}

float gira(float angulo){
	float tf=(3*angulo)/(2*v_max); 
}

int main(int argc, char ** argv){
	ros::init(argc,argv, "carrito_node");
	ros::NodeHandle nh;
	ros::Rate rate(20);

	float delta;

	ros::Time start_time=ros::Time::now();

	float t3;
	
	t3=avanza(30);
	//t3=gira(720);
	float t1=t3/3,t2=t1*2;
	//Se agrego
	ros::Publisher pubVelMotor1 = nh.advertise<std_msgs::Float32>("/speed_motor_1",1);
	ros::Publisher pubVelMotor2 = nh.advertise<std_msgs::Float32>("/speed_motor_2",1);
	while(ros::ok()){
		
		gira();
		avanza();

		std_msgs::Float32 msgI;
		std_msgs::Float32 msgD;	
		if(delta<t1){
			msgI.data =v_max*delta/t1;
			msgD.data =-v_max*delta/t1;
		}
		if(delta>=t1 && delta<t2){
			msgI.data=v_max;
			msgD.data=-v_max;
		}
		if(delta>=t2 && delta<=t3){
			msgI.data=(v_max-((v_max*(delta-t2))/(t3-t2)));
			msgD.data=-(v_max-((v_max*(delta-t2))/(t3-t2)));
		}
		if(delta>t3){
			msgI.data=0;
			msgD.data=0;
		}
		pubVelMotor1.publish(msgI);
		pubVelMotor2.publish(msgD);
		
		ros::spinOnce();
		rate.sleep();

		ros::Duration delta_t= ros::Time::now() - start_time;
		delta= (delta_t.toSec())*1000;
	}
	
	return 1;
}
