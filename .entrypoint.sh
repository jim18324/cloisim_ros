#!/bin/bash
set -e
source "/opt/lge-ros2/install/setup.bash"
exec ros2 launch sim_device_bringup "$@"