#!/bin/bash


cd build
source ../venv/bin/activate

rm -rf ../coverage && mkdir ../coverage
gcovr -f ../engine/src -r . --exclude-throw-branches --html --html-details --output ../coverage/cov.html

