#!/bin/bash

SRC=$(dirname "$0")/../
cppcheck --enable=warning,style,performance $SRC/engine/*
