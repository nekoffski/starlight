#!/bin/bash

if ! [ -d ./build ]; then
    mkdir ./build
fi;

conan install ./conan/ --output-folder=../build --build=missing --profile ./conan/profiles/debug

ls -al

cd build

ls -al

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build . -j8
