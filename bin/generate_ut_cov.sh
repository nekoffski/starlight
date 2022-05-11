#!/bin/sh

sudo apt-get install python3
sudo python3 -m pip install gcovr

cd cov_build

ls /usr/bin | grep gcov

python3 -m gcovr --version
python3 -m gcovr -r . -f ../engine -f ../editor --gcov-executable /usr/bin/gcov-10 --exclude-throw-branches --xml-pretty --output ../coverage.xml

cat ../coverage.xml
