#!/usr/bin/env bash

cmake .
intercept-build make
make clean
cd ..
