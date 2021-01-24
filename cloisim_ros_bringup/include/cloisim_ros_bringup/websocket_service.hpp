/**
 *  @file   websocket_service.hpp
 *  @date   2021-01-21
 *  @author Hyunseok Yang
 *  @brief
 *        websocket service for cloisim
 *  @copyright
 *      LGE Advanced Robotics Laboratory
 *      Copyright (c) 2020 LG Electronics Inc., LTD., Seoul, Korea
 *      All Rights are Reserved.
 *
 *      SPDX-License-Identifier: MIT
 */

#ifndef _CLOISIM_ROS_WEBSOCKET_SERVICE_HPP_
#define _CLOISIM_ROS_WEBSOCKET_SERVICE_HPP_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <jsoncpp/json/json.h>

typedef websocketpp::client<websocketpp::config::asio_client> client;

namespace cloisim_ros
{
  using namespace std;

  class WebSocketService
  {
  public:
    explicit WebSocketService(const string bridge_ip, const string service_port);

  private:
    Json::Reader reader;

    string uri;
    client c; // Create a client endpoint

    string target_filter;
    Json::Value result;

  private:
    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg);
    void on_open(websocketpp::connection_hdl hdl);

  public:
    void SetTarget(const string target)
    {
      target_filter = target;
    }

    Json::Value Run();
  };
}

#endif