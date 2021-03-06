/**
 *  @file   depthcamera.hpp
 *  @date   2021-01-14
 *  @author Sungkyu Kang
 *  @author hyunseok Yang
 *  @brief
 *        ROS2 Depth Camera class for simulator
 *  @remark
 *  @warning
 *       LGE Advanced Robotics Laboratory
 *         Copyright(C) 2019 LG Electronics Co., LTD., Seoul, Korea
 *         All Rights are Reserved.
 */

#ifndef _CLOISIM_ROS_DEPTHCAMERA_HPP_
#define _CLOISIM_ROS_DEPTHCAMERA_HPP_

#include <cloisim_ros_camera/camera.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

namespace cloisim_ros
{
  class DepthCamera : public Camera
  {
  public:
    explicit DepthCamera(const rclcpp::NodeOptions &options_, const std::string node_name_, const std::string namespace_ = "");
    explicit DepthCamera(const std::string node_name_ = "cloisim_ros_depthcamera", const std::string namespace_ = "");
    virtual ~DepthCamera();

  private:
    virtual void Initialize() override;
    virtual void Deinitialize() override;
    virtual void UpdateData(const uint bridge_index) override;

  private:
    // Camera info publisher
    // rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr pubDepthCameraInfo{nullptr};

    // Store current point cloud.
    // sensor_msgs::msg::PointCloud2 msg_pc2;

    // Point cloud publisher.
    // rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pubPointCloud;
  };
}
#endif