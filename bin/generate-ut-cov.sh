#!/bin/bash


cd build
source ../venv/bin/activate

rm -rf ../coverage && mkdir ../coverage
gcovr -f ../engine/src -r . --exclude-throw-branches --xml --output ../coverage/coverage.xml
