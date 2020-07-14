#!/bin/sh

if [ -d ./build ]; then
    rm build -rf
fi

mkdir ./build
cd ./build

CC=gcc-10 CXX=g++-10 cmake ..
make -j 4
