#!/bin/bash

git submodule update --init --recursive

imgui_dir='3rdparty/imgui'
cp $imgui_dir/backends/{imgui_impl_glfw*,imgui_impl_opengl3*} $imgui_dir
