#!/bin/bash
SRC=$(dirname "$0")/../

sudo add-apt-repository universe
sudo apt-get update

sudo apt-get install -y python3
sudo apt-get install -y cppcheck 
sudo apt-get install -y libfreetype-dev 

#!/bin/bash

if ! [ -d ./venv ]; then
    python -m venv ./venv
fi

source ${SRC}/venv/bin/activate
python -m pip install -r ./requirements.txt

conan profile detect --force

git clone https://github.com/nekoffski/kstd
pushd kstd
chmod +x ./bin/*
./bin/deps.sh
./bin/create_package.sh
popd

conan install ${SRC} --output-folder=${SRC} --build=missing --profile ${SRC}/config/profiles/debug
