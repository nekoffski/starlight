#!/bin/sh

sudo apt-get update
sudo apt-get install -y gcc-10 g++-10
sudo apt-get install -y libxrandr-dev libxinerama-dev libx11-dev libxcursor-dev libxi-dev libgl-dev \
    uuid-dev libassimp-dev libglfw3-dev libjsoncpp-dev libglm-dev libgtest-dev libgmock-dev libfmt-dev vulkansdk

if [ -d ./3rdparty/libkc ]; then
    chmod +x ./3rdparty/libkc/bin/deps.sh
    ./3rdparty/libkc/bin/deps.sh
fi;
