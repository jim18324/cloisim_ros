/**
 *  @file   cloisim_ros_world.hpp
 *  @date   2021-01-14
 *  @author Hyunseok Yang
 *  @brief
 *        ROS2 node for controlling unity simulation
 *  @remark
 *        Gazebonity
 *  @copyright
 *      LGE Advanced Robotics Laboratory
 *      Copyright (c) 2020 LG Electronics Inc., LTD., Seoul, Korea
 *      All Rights are Reserved.
 *
 *      SPDX-License-Identifier: MIT
 */

#ifndef _CLOISIM_ROS_WORLD_HPP_
#define _CLOISIM_ROS_WORLD_HPP_

#include <cloisim_ros_base/base.hpp>
#include <rosgraph_msgs/msg/clock.hpp>

namespace cloisim_ros
{
  class Throttler;

  class World : public Base
  {
  public:
    explicit World(const rclcpp::NodeOptions &options_, const std::string node_name_);
    explicit World();
    virtual ~World();

  private:
    virtual void Initialize() override;
    virtual void Deinitialize() override;
    virtual void UpdateData(const uint bridge_index) override;

    void PublishSimTime(const rclcpp::Time simTime, const rclcpp::Time realTime);

  private:
    Throttler *throttler_;

    std::string hashKeySub_;

    cloisim::msgs::Param pbBuf_;

    rclcpp::Publisher<rosgraph_msgs::msg::Clock>::SharedPtr clock_pub_;
  };

  class Throttler
  {
  private:
    double period_;
    rclcpp::Time last_time_;

  public:
    Throttler(const double _hz)
        : period_(1.0 / _hz),
          last_time_(0)
    {
    }

    bool IsReady(const rclcpp::Time &_now)
    {
      // If time went backwards, reset
      if (_now < last_time_)
      {
        // printf("backward %.10f, %.10f\n", last_time_.seconds(), _now.seconds());
        last_time_ = _now;
        return true;
      }

      // If not enough time has passed, return false
      double elapsedTime = (_now - last_time_).seconds();

      if (period_ > 0 && elapsedTime < period_)
      {
        // printf("FALSE %.10f, %.10f\n", elapsedTime, _now.seconds());
        return false;
      }

      // Enough time has passed, set last time to now and return true
      last_time_ = _now;
      // printf("TRUE %.10f , %.10f\n", last_time_.seconds(),  _now.seconds());
      return true;
    }
  };
}
#endif