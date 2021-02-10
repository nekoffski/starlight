#!/bin/sh

sudo apt-get install python3.7
sudo python3.7 -m pip install gcovr

cd cov_build

ls /usr/bin | grep gcov

python3.7 -m gcovr --version
python3.7 -m gcovr -r . -f ../src --gcov-executable /usr/bin/gcov-10 --exclude-throw-branches --xml-pretty --output ../coverage.xml

cat ../coverage.xml
