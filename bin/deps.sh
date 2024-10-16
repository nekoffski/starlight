#!/bin/bash

sudo apt-get update

if [ -d ./3rdparty/libkc ]; then
    chmod +x ./3rdparty/libkc/bin/deps.sh
    ./3rdparty/libkc/bin/deps.sh
fi;

sudo apt-get install -y libxrandr-dev 
sudo apt-get install -y libxinerama-dev 
sudo apt-get install -y libx11-dev 
sudo apt-get install -y libxcursor-dev 
sudo apt-get install -y libxi-dev 
sudo apt-get install -y libgl-dev 
sudo apt-get install -y uuid-dev 
sudo apt-get install -y libassimp-dev 
sudo apt-get install -y libglfw3-dev 
sudo apt-get install -y libjsoncpp-dev 
sudo apt-get install -y libglm-dev 
sudo apt-get install -y libgtest-dev 
sudo apt-get install -y libgmock-dev 
sudo apt-get install -y libfmt-dev 
sudo apt-get install -y libassimp-dev
sudo apt-get install -y libfreetype-dev 
sudo apt-get install -y cppcheck 
sudo apt-get install -y libboost-dev

# show vulkan version
apt list --installed | grep vulkan
