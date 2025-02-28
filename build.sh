#!/bin/bash

if [ ! -d "build" ]; then
    mkdir build;
fi
cd build;
cmake ../src;
cmake --build .;