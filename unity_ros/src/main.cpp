/**
 *  @file   main.cpp
 *  @date   2020-04-08
 *  @author Hyunseok Yang
 *  @brief
 *        ROS2 packages that helps to control unity simulation
 *  @remark
 *  @copyright
 *      LGE Advanced Robotics Laboratory
 *      Copyright (c) 2020 LG Electronics Inc., LTD., Seoul, Korea
 *      All Rights are Reserved.
 *
 *      SPDX-License-Identifier: MIT
 */
#include "unity_ros/unity_ros.hpp"

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<UnityRos::UnityRosInit>();
  rclcpp::spin(node);
  rclcpp::sleep_for(100ms);
  rclcpp::shutdown();
  return 0;
}