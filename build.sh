#!/bin/bash

if [ $# -ge 1 ];then
  if [ $1 == "clean" ];then
    rm -rf build output
    exit
  fi
fi

mkdir -p build && cd build && cmake .. -DCMAKE_INSTALL_PREFIX=../output && make -j8 install
