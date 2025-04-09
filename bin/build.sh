#!/bin/bash

SRC=$(dirname "$0")/../
rm ${SRC}/build -rf
conan build . --profile ${SRC}/config/profiles/debug