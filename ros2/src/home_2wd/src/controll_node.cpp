#include <cstdio>
#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>
#include <sensor_msgs/msg/joy.hpp>

using namespace std::chrono_literals;
using std::placeholders::_1;
using std::to_string;
const double PI=3.141592653589;

class Cmd2Wheel2 : public rclcpp::Node{
  const float InvSqrt2 = pow(2.0,-0.5);//power(2.0, 0.5)
  const float PARAM_LINE = 3000;
  const float PARAM_ANGL  = 3000;

  //subとmainで共有する変数
  float cmd_velo_;
  float cmd_rote_;


  public:
  Cmd2Wheel2(): Node("cmd2_wheel2_node"){
    sub_joy_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "joy", 10, std::bind(&Cmd2Wheel2::sub_callback_joy, this, _1));

      pub_str_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);
      pub_whl_ = this->create_publisher<geometry_msgs::msg::Twist>("serial_send", 10);

      timer_ = this->create_wall_timer(
        50ms, std::bind(&Cmd2Wheel2::main_callback_loop, this));
  }

  void main_callback_loop(){
    auto msg_whl = geometry_msgs::msg::Twist();
    msg_whl.linear.x  = cmd_velo_ * 255.0 * InvSqrt2 + cmd_rote_ * 255.0 * InvSqrt2 ; //L
    msg_whl.linear.y  = cmd_velo_ * 255.0 * InvSqrt2 - cmd_rote_ * 255.0 * InvSqrt2 ; //R

    RCLCPP_INFO(this->get_logger(), "y = %f, z = %f\n", cmd_velo_, cmd_rote_);
    pub_whl_->publish(msg_whl);
  }

  void sub_callback_joy(const sensor_msgs::msg::Joy& msg){
    float JoyLY = msg.axes[1] ;//-1.0~1.0
    float JoyRX = msg.axes[3];//-1.0~1.0
    float JoyAbsLength = pow((pow(JoyLY,2.0) + pow(JoyRX,2.0)), 0.5);

    if(JoyAbsLength >  1.0) JoyAbsLength = 1.0;
    if(JoyAbsLength < -1.0) JoyAbsLength = -1.0;
    
    float JoyRadian = atan2(JoyLY, JoyRX);

    cmd_velo_ = JoyAbsLength * sin(JoyRadian);
    cmd_rote_ = JoyAbsLength * cos(JoyRadian);

  }


  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr     sub_joy_;
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
