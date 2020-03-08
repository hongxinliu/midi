#!/bin/bash

if [ $# -ge 1 ];then
  if [ $1 == "clean" ];then
    rm -rf build output
    exit
  fi
fi

if [ -z ${Qt5_DIR} ];then
  echo "Qt5_DIR is not set"
fi

mkdir -p build && cd build && cmake .. -DCMAKE_INSTALL_PREFIX=../output -DQt5_DIR=${Qt5_DIR} && make -j8 install
