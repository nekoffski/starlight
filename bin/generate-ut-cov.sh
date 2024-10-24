#!/bin/bash


cd build
source ../venv/bin/activate
gcovr -r . -f ../engine/src --exclude-throw-branches --html --output ../coverage.html
