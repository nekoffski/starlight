#!/bin/sh

if [ -d ./build ]; then
    rm build -rf
fi

mkdir ./build
cd ./build

CC=/usr/bin/gcc-10 CXX=/usr/bin/g++-10 cmake ..
make -j `nproc`
