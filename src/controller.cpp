#include <memory>
#include <chrono>
#include <cinttypes>

//add the ROS header
#include "rclcpp/rclcpp.hpp"
//add the headers for 
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/float32.hpp"


using std::placeholders::_1;

class MyController : public rclcpp::Node
{
public:
  MyController()
  : Node("my_controller")
  { 
    publisher_left_ = this->create_publisher<std_msgs::msg::Float32>("/leftwheel_vel", 1);
    publisher_right_ = this->create_publisher<std_msgs::msg::Float32>("/rightwheel_vel", 1);
    subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
      "/proxy_sensor", 10, std::bind(&MyController::sensor_callback, this, _1));
  }
      
private:

  void sensor_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
  {
    auto vel_l = std_msgs::msg::Float32();
    auto vel_r = std_msgs::msg::Float32();
    
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
	publisher_left_->publish(vel_l);
	publisher_right_->publish(vel_r);
  }
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_left_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_right_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MyController>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
