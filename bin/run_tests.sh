#!/bin/sh

if ! [ -d ./build/test ]; then
    echo "Tests not builded"
    exit 0
fi

cd ./build

make test