#!/bin/bash

if ! [ -d ./venv ]; then
    python -m venv ./venv
fi

source ./venv/bin/activate
python -m pip install -r ./requirements.txt
