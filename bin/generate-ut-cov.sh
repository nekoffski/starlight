#!/bin/bash


cd build
source ../venv/bin/activate
gcovr -f ../engine/src --exclude-throw-branches --html --output ../coverage.html
