#!/bin/bash

sudo apt-get update

if [ -d ./3rdparty/libkc ]; then
    chmod +x ./3rdparty/libkc/bin/deps.sh
    ./3rdparty/libkc/bin/deps.sh
fi;

sudo apt-get install -y gcc-10 g++-10
sudo apt-get install -y libxrandr-dev libxinerama-dev libx11-dev libxcursor-dev libxi-dev libgl-dev \
    uuid-dev libassimp-dev libglfw3-dev libjsoncpp-dev libglm-dev libgtest-dev libgmock-dev libfmt-dev

wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list
sudo apt update
sudo apt install vulkan-sdk

# show vulkan version
apt list --installed | grep vulkan
