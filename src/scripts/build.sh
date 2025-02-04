#!/bin/bash

cd ..

rm -r build -f

# Create and navigate to the build directory
mkdir -p build
cd build

# Run CMake
cmake ..

# Build the project
make
