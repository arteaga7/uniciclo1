/*uniciclo_node: Se subscribe al topico "/odom" para obtener la postura de un robot waffle
 *en Gazebo y publica en el topico "/cmd_vel" para asignar velocidades lineal y angular.*/
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_broadcaster.h"

//Variables globales
double X1, Y1, roll1, pitch1, yaw1;
geometry_msgs::Twist vel1;     //Crea objeto "vel1" para mensajes tipo "geometry_msgs/Twist"

void odomCallback1(const nav_msgs::Odometry::ConstPtr &msg){
//Mensaje "nav_msgs/Odometry" correspondiente al topico "/odom"
  X1 = msg->pose.pose.position.x;
  Y1 = msg->pose.pose.position.y;
//operaciones para obtener los angulos de Euler a partir de Quaterniones
  tf::Quaternion quater;
  tf::quaternionMsgToTF(msg->pose.pose.orientation, quater);
  tf::Matrix3x3(quater).getRPY(roll1, pitch1, yaw1);  //angulos de Euler [-pi/2,pi/2]
  printf("X1: %.4f, Y1: %.4f, yaw1: %.3f\n",X1,Y1,yaw1);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "uniciclo_node");   //Inicializa el nodo
  ros::NodeHandle n1;                       //Crea el handle
  ros::Publisher  pub1;          //Crea objeto "pub1" de tipo "Publisher"
  ros::Subscriber sub1;          //Crea objeto "sub1" de tipo "Subscriber"
  pub1 = n1.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
//Mensaje "geometry_msgs/Twist" correspondiente al topico "/cmd_vel", buffer=1
  sub1 = n1.subscribe("/odom", 1, odomCallback1);
//Topico "/odom", buffer=1, funcion de callback
  ros::Rate loop_rate(10);      //Frecuencia de ejecucion=10hz

  while(ros::ok()){
    vel1.linear.x = 0.1;
    vel1.angular.z = 0.2;
    pub1.publish(vel1);
    ros::spinOnce();            //Activa callbacks
    loop_rate.sleep();          //Espera hasta completar el loop rate
  }
  return 0;
}
