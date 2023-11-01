#!/bin/bash

if ! [ -d ./venv ]; then
    python3.10 -m venv ./venv
fi

source ./venv/bin/activate
python3.10 -m pip install -r ./requirements.txt
