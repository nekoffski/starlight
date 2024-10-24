#!/bin/bash

SRC=$(dirname "$0")/../

conan install ${SRC}/conan/ --output-folder=${SRC}/build --build=missing --profile ${SRC}/conan/profiles/debug

cd ${SRC}/build

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DSL_ENABLE_UNIT_TESTS=On \
    -DSL_ENABLE_COVERAGE=On -DSL_ENABLE_BENCHMARKS=On
cmake --build . -j`nproc`
