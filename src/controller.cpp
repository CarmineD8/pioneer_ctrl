#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Float32.h"

ros::Publisher pub_left;
ros::Publisher pub_right;

void sensorCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{

	std_msgs::Float32 vel_l;
	std_msgs::Float32 vel_r;
	bool obstacle=false;
	bool ob_left=false;
	bool ob_front=false;
	bool ob_right=false;
	for(int i=0;i<8;i++){
		if (msg->intensities[i]>0.0){
			if(i<3)
			{
			ob_left=true;
			}
			else if (i<5)
			{
			ob_front=true;
			}
			else {
			ob_right=true;
			}
			obstacle=true;
		}
	}
	if(obstacle==false){
		vel_l.data=1.0;
		vel_r.data=1.0;
	}
	else{
		if(ob_front & ob_right & ob_left){
		vel_l.data=1.0;
		vel_r.data=-1.0;
		}
		else if (ob_right & ob_left){
		vel_l.data=1.0;
		vel_r.data=-1.0;
		}
		else if (ob_right){
		vel_l.data=0.0;
		vel_r.data=1.0;
		}
		else{
		vel_l.data=1.0;
		vel_r.data=0.0;
		}
	
	
	}
	pub_left.publish(vel_l);
	pub_right.publish(vel_r);
}

int main(int argc, char **argv)
{
   ros::init(argc, argv, "turtlebot_controller");
   ros::NodeHandle n;
   
   pub_left = n.advertise<std_msgs::Float32>("/leftwheel_vel", 1000);
   pub_right = n.advertise<std_msgs::Float32>("/rightwheel_vel", 1000);
   
   ros::Subscriber sub = n.subscribe("/proxy_sensor", 1000, sensorCallback);

   ros::spin();

   return 0;
}
