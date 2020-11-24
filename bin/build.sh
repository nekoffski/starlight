#!/bin/sh

if [ -d ./build ]; then
    rm build -rf
fi

cd 3rdparty/fuzzylite
cmake ./fuzzylite
make -j 16
cd ../..

mkdir ./build
cd ./build

CC=/usr/bin/gcc-10 CXX=/usr/bin/g++-10 cmake ..
make -j 16
