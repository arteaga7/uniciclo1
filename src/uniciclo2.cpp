/*uniciclo2_node: Se subscribe al topico "/odom" para obtener la postura de 2 robots
 * waffle y burger en Gazebo y publica en los topicos "/cmd_vel" para asignar velocidades
 * lineal y angular.*/
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_broadcaster.h"

//Variables globales
double X1, Y1, roll1, pitch1, yaw1, X2, Y2, roll2, pitch2, yaw2;
geometry_msgs::Twist vel1;     //Crea objeto "vel1" para mensajes tipo "geometry_msgs/Twist"
geometry_msgs::Twist vel2;     //Crea objeto "vel2" para mensajes tipo "geometry_msgs/Twist"

void odomCallback1(const nav_msgs::Odometry::ConstPtr &msg){
//Mensaje "nav_msgs/Odometry" correspondiente al topico "/odom"
  X1 = msg->pose.pose.position.x;
  Y1 = msg->pose.pose.position.y;
//operaciones para obtener los angulos de Euler a partir de Quaterniones
  tf::Quaternion quater;
  tf::quaternionMsgToTF(msg->pose.pose.orientation, quater);
  tf::Matrix3x3(quater).getRPY(roll1, pitch1, yaw1);  //angulos de Euler [-pi/2,pi/2]
}

void odomCallback2(const nav_msgs::Odometry::ConstPtr &msg){
//Mensaje "nav_msgs/Odometry" correspondiente al topico "/odom"
  X2 = msg->pose.pose.position.x;
  Y2 = msg->pose.pose.position.y;
//operaciones para obtener los angulos de Euler a partir de Quaterniones
  tf::Quaternion quater;
  tf::quaternionMsgToTF(msg->pose.pose.orientation, quater);
  tf::Matrix3x3(quater).getRPY(roll2, pitch2, yaw2);  //angulos de Euler [-pi/2,pi/2]
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "uniciclo2_node");   //Inicializa el nodo
  ros::NodeHandle n1,n2;                     //Crea los handles
  ros::Publisher  pub1,pub2;          //Crea objetos "pub1" y "pub2" de tipo "Publisher"
  ros::Subscriber sub1,sub2;          //Crea objetos "sub1" y "sub2" de tipo "Subscriber"
  pub1 = n1.advertise<geometry_msgs::Twist>("/r1/cmd_vel", 1);
  pub2 = n2.advertise<geometry_msgs::Twist>("/r2/cmd_vel", 1);
//Mensaje "geometry_msgs/Twist" correspondiente al topico "/cmd_vel", buffer=1
  sub1 = n1.subscribe("/r1/odom", 1, odomCallback1);
  sub2 = n2.subscribe("/r2/odom", 1, odomCallback2);
//Topico "/odom", buffer=1, funcion de callback
  ros::Rate loop_rate(10);      //Frecuencia de ejecucion=10hz

  while(ros::ok()){
    vel1.linear.x = 0.1;
    vel1.angular.z = 0.2;
    pub1.publish(vel1);

    vel2.linear.x = 0.1;
    vel2.angular.z = 0.3;
    pub2.publish(vel2);
    ros::spinOnce();            //Activa callbacks
    loop_rate.sleep();          //Espera hasta completar el loop rate
  }
  return 0;
}
