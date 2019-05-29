// Cabeceras para ros
#include <ros/ros.h>

// Cabeceras para mensajes estandar
#include <std_msgs/Int64.h>
#include <std_msgs/Float32.h>

// Cabeceras para el estado de los joints
#include <sensor_msgs/JointState.h>

// Cabeceras para realizar las transformaciones
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

// Este objeto nos sirve para realizar transformaciones entre frames
tf::TransformListener * tf_listener;

// Funciones para los encoders

void callbackEncoder1(const std_msgs::Int64::ConstPtr &msg){
	std::cout << msg->data << std::endl;
}

void callbackEncoder2(const std_msgs::Int64::ConstPtr &msg){
	std::cout << msg->data << std::endl;
}

int main(int argc, char ** argv){
	
	//Inicializacion del nodo
	ros::init(argc, argv, "mobile_base_node");
	
	//Se crea el mando del nodo
	ros::NodeHandle nh;
	
	//Se crea el objeto que nos permite realizar el muestreo de los mensajes
	ros::Rate rate(30);

	// Se crean los subscriptores de los mensajes
	ros::Subscriber subEncoder1 = nh.subscribe("/encoder_1", 1, callbackEncoder1);
	ros::Subscriber subEncoder2 = nh.subscribe("/encoder_2", 1, callbackEncoder2);

	// Publicador del estado de los joints
	ros::Publisher pubJointState = nh.advertise<sensor_msgs::JointState>("/joint_states", 1);

	// Se instancia el objeto para realizar transformaciones
	tf_listener = new tf::TransformListener();

	// Nombre de los joints
	std::string jointNames[2] = {"left_wheel_joint_connect", "right_wheel_joint_connect"};
	float jointPositions[2] = {0.0, 0.0};
	sensor_msgs::JointState jointState;

	// Se asignan los nombres de los joints y las dimensiones

	jointState.name.insert(jointState.name.begin(), jointNames, jointNames + 2);
	jointState.position.insert(jointState.position.begin(), jointPositions, jointPositions + 2);
	
	// El objeto que envía las transformaciones
	tf::TransformBroadcaster br;

	// Loop de ros
	while(ros::ok()){
	
		// Transformaciones
		tf::Transform transform;

		// TODO Se calcula la odometria
		transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
		tf::Quaternion q;
		q.setRPY(0, 0, 0);
		transform.setRotation(q);
		
		// Se envia la transformaciones del base_link al odom
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", "odom"));
		
		// Se Publican el estado de los joints
		pubJointState.publish(jointState);
		rate.sleep();
		ros::spinOnce();
	}
}
