#include <cstdio>
#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;
using std::placeholders::_1;
using std::to_string;
const double PI=3.141592653589;

class Cmd2Wheel2 : public rclcpp::Node{

  float cmd_y;
  float cmd_z;
  const float InvSqrt2 = pow(2.0,-0.5);//power(2.0, 0.5)
  const float PARAM_LINE = 3000;
  const float PARAM_ANGL  = 3000;

  float cmd_length;
  float cmd_radian;
  float cmd_dash_y;
  float cmd_dash_z;




public:
  Cmd2Wheel2(): Node("cmd2_wheel2_node"){
    sub_cmd_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "cmd_vel", 10, std::bind(&Cmd2Wheel2::sub_callback_cmd, this, _1));

      pub_str_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
      pub_whl_ = this->create_publisher<geometry_msgs::msg::Twist>("serial_send", 10);

      timer_ = this->create_wall_timer(
        50ms, std::bind(&Cmd2Wheel2::main_callback_loop, this));
  }

  void main_callback_loop(){
    auto msg_whl = geometry_msgs::msg::Twist();
    msg_whl.linear.x  = cmd_dash_y * 255.0 * InvSqrt2 + cmd_dash_z * 255.0 * InvSqrt2 ; //L
    msg_whl.linear.y  = cmd_dash_y * 255.0 * InvSqrt2 - cmd_dash_z * 255.0 * InvSqrt2 ; //R

    RCLCPP_INFO(this->get_logger(), "y = %f, z = %f\n", cmd_dash_y, cmd_dash_z);
    pub_whl_->publish(msg_whl);
  }

  void sub_callback_cmd(const geometry_msgs::msg::Twist& msg){
    cmd_y = msg.linear.y ;//-1.0~1.0
    cmd_z = msg.angular.z;//-1.0~1.0
    cmd_length = pow((pow(cmd_y,2.0) + pow(cmd_z,2.0)), 0.5);

    if(cmd_length >  1.0) cmd_length = 1.0;
    if(cmd_length < -1.0) cmd_length = -1.0;
    
    cmd_radian = atan2(cmd_y, cmd_z);

    cmd_dash_y = cmd_length * sin(cmd_radian);
    cmd_dash_z = cmd_length * cos(cmd_radian);

  }


  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr        pub_str_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr    pub_whl_;
  rclcpp::TimerBase::SharedPtr timer_;

};


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Cmd2Wheel2>());
  rclcpp::shutdown();
  return 0;
}
