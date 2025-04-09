#!/bin/bash

SRC=$(dirname "$0")/../
rm ${SRC}/build -rf

source ${SRC}/venv/bin/activate
conan build . --profile ${SRC}/config/profiles/debug