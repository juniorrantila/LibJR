#!/bin/sh

mkdir -p Build/Tests &&
CXX=clang++ cmake -B Build -S . -G Ninja -DBUILD_TESTS=YES > /dev/null &&
ninja -C Build &&
./Build/Tests/run-tests ./Build/Tests
