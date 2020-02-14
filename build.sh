#!/bin/sh

if [ -d ./build ]; then
    rm build -rf
fi

mkdir ./build
cd ./build

cmake ..
make -j 4
