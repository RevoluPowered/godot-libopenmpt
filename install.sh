#!/bin/bash
[ -z "$1" ] && echo "Usage: ./install.sh <godot-project-path>" && exit 1
mkdir -p "$1/addons/libopenmpt"
cp -r demo/bin "$1/addons/libopenmpt/"
echo "Installed to $1/addons/libopenmpt"
