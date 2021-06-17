#!/bin/bash

[ -d "build" ] && echo "Build folder already exists!" && cd build
[ ! -d "build" ] && echo "Build folder does not exist. Creating now." &&  mkdir build && cd build

build_type = $1
release = "Release"

[if $build_type == $release]
then
    cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Release .. && echo "Building Release configuration."
else
    cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug .. && echo "Building Debug configuration."
fi

make
