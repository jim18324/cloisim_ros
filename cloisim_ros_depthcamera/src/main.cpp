/**
 *  @file   main.cpp
 *  @date   2021-01-14
 *  @author Sungkyu Kang
 *  @brief
 *        ROS2 Camera class for simulator
 *  @remark
 *  @warning
 *      LGE Advanced Robotics Laboratory
 *      Copyright (c) 2020 LG Electronics Inc., LTD., Seoul, Korea
 *      All Rights are Reserved.
 *
 *      SPDX-License-Identifier: MIT
 */

#include "cloisim_ros_depthcamera/depthcamera.hpp"

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<cloisim_ros::DepthCamera>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
