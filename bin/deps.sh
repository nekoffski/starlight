#!/bin/bash

sudo add-apt-repository universe
sudo apt-get update

if [ -d ./3rdparty/libkc ]; then
    chmod +x ./3rdparty/libkc/bin/deps.sh
    ./3rdparty/libkc/bin/deps.sh
fi;

sudo apt-get install -y python3
sudo apt-get install -y cppcheck 
sudo apt-get install -y libboost-dev
sudo apt-get install -y libfreetype-dev 

pip install conan && conan profile detect --force
