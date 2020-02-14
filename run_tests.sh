#!/bin/sh

if ! [ -d ./build/test ]; then
    echo "Tests not builded"
    exit 0
fi

./build/test/sl_core_test

