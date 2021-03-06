###############################################################################
# Set minimum required version of cmake, project name and compile options
################################################################################
cmake_minimum_required(VERSION 3.5)
project(cloisim_ros_bringup)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fstack-protector -O2)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

################################################################################
# Find colcon packages and libraries for colcon and system dependencies
################################################################################
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(websocketpp REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(cloisim_ros_camera REQUIRED)
find_package(cloisim_ros_micom REQUIRED)
find_package(cloisim_ros_lidar REQUIRED)
find_package(cloisim_ros_camera REQUIRED)
find_package(cloisim_ros_depthcamera REQUIRED)
find_package(cloisim_ros_multicamera REQUIRED)
find_package(cloisim_ros_realsense REQUIRED)
find_package(cloisim_ros_gps REQUIRED)
find_package(cloisim_ros_elevatorsystem REQUIRED)
find_package(cloisim_ros_world REQUIRED)

set(dependencies
  rclcpp
  websocketpp
  cloisim_ros_camera
  cloisim_ros_micom
  cloisim_ros_lidar
  cloisim_ros_camera
  cloisim_ros_depthcamera
  cloisim_ros_multicamera
  cloisim_ros_realsense
  cloisim_ros_gps
  cloisim_ros_elevatorsystem
  cloisim_ros_world
)

################################################################################
# Build
################################################################################
add_executable(
  ${PROJECT_NAME}
  src/websocket_service.cpp
  src/main.cpp
)

target_include_directories(
  ${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(
  ${PROJECT_NAME}
  ${dependencies}
)

target_link_libraries(
  ${PROJECT_NAME}
  jsoncpp
)

################################################################################
# Install
################################################################################
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

ament_package()