/**
 *  @file   main.cpp
 *  @date   2021-01-14
 *  @author Hyunseok Yang
 *  @brief
 *        ROS2 package for elevator system
 *  @remark
 *  @copyright
 *      LGE Advanced Robotics Laboratory
 *      Copyright (c) 2020 LG Electronics Inc., LTD., Seoul, Korea
 *      All Rights are Reserved.
 *
 *      SPDX-License-Identifier: MIT
 */

#include "cloisim_ros_elevatorsystem/elevatorsystem.hpp"

using namespace std::literals;

int main(int argc, char *argv[])
{
  // Force flush of the stdout buffer.
  // This ensures a correct sync of all prints
  // even when executed simultaneously within the launch file.
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);
  auto node = std::make_shared<cloisim_ros::ElevatorSystem>();
  rclcpp::sleep_for(5000ms);
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();

  return 0;
}