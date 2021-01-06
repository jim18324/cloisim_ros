/**
 *  @file   MultiCameraDriverSim.cpp
 *  @date   2020-05-20
 *  @author hyunseok Yang
 *  @author Sungkyu Kang
 *  @brief
 *        ROS2 Multi Camera Driver class for simulator
 *  @remark
 *  @warning
 *       LGE Advanced Robotics Laboratory
 *         Copyright(C) 2019 LG Electronics Co., LTD., Seoul, Korea
 *         All Rights are Reserved.
 */
#include "multi_camera_driver_sim/MultiCameraDriverSim.hpp"
#include <sensor_msgs/fill_image.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <driver_sim/helper.h>
#include <protobuf/param.pb.h>

using namespace std;
using namespace chrono_literals;
using namespace gazebo;

MultiCameraDriverSim::MultiCameraDriverSim(const string node_name)
    : DriverSim(node_name, 2)
{
  Start();
}

MultiCameraDriverSim::~MultiCameraDriverSim()
{
  Stop();
}

void MultiCameraDriverSim::Initialize()
{
  uint16_t portInfo;
  get_parameter_or("bridge.Data", portData_, uint16_t(0));
  get_parameter_or("bridge.Info", portInfo, uint16_t(0));

  multicamera_name_ = get_name();

  tf2::Quaternion fixed_rot;
  m_hashKeySub = GetMainHashKey();
  DBG_SIM_INFO("hash Key sub: %s", m_hashKeySub.c_str());

  auto pSimBridgeData = GetSimBridge(0);
  auto pSimBridgeInfo = GetSimBridge(1);

  if (pSimBridgeInfo != nullptr)
  {
    pSimBridgeInfo->Connect(SimBridge::Mode::CLIENT, portInfo, m_hashKeySub + "Info");

    GetRos2FramesId(pSimBridgeInfo);
  }

  image_transport::ImageTransport it(GetNode());
  for (auto frame_id : frame_id_)
  {
    const auto transform = GetObjectTransform(pSimBridgeInfo, frame_id);
    SetupStaticTf2(transform, multicamera_name_ + "_" + frame_id);

    // Image publisher
    const auto topic_base_name_ = multicamera_name_ + "/" + frame_id;
    DBG_SIM_INFO("topic_base_name:%s", topic_base_name_.c_str());

    sensor_msgs::msg::Image msg_img;
    msg_img.header.frame_id = frame_id;

    msg_imgs_[msg_imgs_.size()] = msg_img;

    pubImages_.push_back(it.advertiseCamera(topic_base_name_ + "/image_raw", 1));

    // TODO: to supress the error log
    // -> Failed to load plugin image_transport/compressed_pub, error string: parameter 'format' has already been declared
    // -> Failed to load plugin image_transport/compressed_pub, error string: parameter 'png_level' has already been declared
    // -> Failed to load plugin image_transport/compressed_pub, error string: parameter 'jpeg_quality' has already been declared
    undeclare_parameter("format");
    undeclare_parameter("png_level");
    undeclare_parameter("jpeg_quality");

    cameraInfoManager.push_back(std::make_shared<camera_info_manager::CameraInfoManager>(GetNode().get()));
    const auto camSensorMsg = GetCameraSensorMessage(pSimBridgeInfo, frame_id);
    SetCameraInfoInManager(cameraInfoManager.back(), camSensorMsg, frame_id);
  }

  pSimBridgeData->Connect(SimBridge::Mode::SUB, portData_, m_hashKeySub + "Data");
}

void MultiCameraDriverSim::Deinitialize()
{
  for (auto pub : pubImages_)
  {
    pub.shutdown();
  }

  DisconnectSimBridges();
}

void MultiCameraDriverSim::GetRos2FramesId(SimBridge* const pSimBridge)
{
  if (pSimBridge == nullptr)
  {
    return;
  }

  msgs::Param request_msg;
  request_msg.set_name("request_ros2");

  msgs::Param reply_msg = RequestReplyMessage(pSimBridge, request_msg);

  if (reply_msg.IsInitialized())
  {
    if (reply_msg.name() == "ros2")
    {
      auto baseParam = reply_msg.children(0);
      if (baseParam.name() == "frames_id")
      {
        for (auto i = 0; i < baseParam.children_size(); i++)
        {
          auto param = baseParam.children(i);
          if (param.name() == "frame_id" && param.has_value() &&
              param.value().type() == msgs::Any_ValueType_STRING &&
              !param.value().string_value().empty())
          {
            const auto frame_id = param.value().string_value();
            frame_id_.push_back(frame_id);
            DBG_SIM_INFO("frame_id: %s", frame_id.c_str());
          }
        }
      }
    }
  }
}

void MultiCameraDriverSim::UpdateData(const uint bridge_index)
{
  void *pBuffer = nullptr;
  int bufferLength = 0;

  const bool succeeded = GetBufferFromSimulator(bridge_index, &pBuffer, bufferLength);
  if (!succeeded || bufferLength < 0)
  {
    return;
  }

  if (!m_pbBuf.ParseFromArray(pBuffer, bufferLength))
  {
    DBG_SIM_ERR("Parsing error, size(%d)", bufferLength);
    return;
  }

  m_simTime = rclcpp::Time(m_pbBuf.time().sec(), m_pbBuf.time().nsec());

  for (auto i = 0; i < m_pbBuf.image_size(); i++)
  {
    auto img = &m_pbBuf.image(i);

    const auto encoding_arg = GetImageEncondingType(img->pixel_format());
    const uint32_t cols_arg = img->width();
    const uint32_t rows_arg = img->height();
    const uint32_t step_arg = img->step();

    auto const msg_img = &msg_imgs_[i];
    msg_img->header.stamp = m_simTime;
    sensor_msgs::fillImage(*msg_img, encoding_arg, rows_arg, cols_arg, step_arg,
                           reinterpret_cast<const void *>(img->data().data()));

    // Publish camera info
    auto camera_info_msg = cameraInfoManager[i]->getCameraInfo();
    camera_info_msg.header.stamp = m_simTime;

    pubImages_.at(i).publish(*msg_img, camera_info_msg);
  }
}