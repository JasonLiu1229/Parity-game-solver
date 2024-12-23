#!/bin/bash

# Create and navigate to the build directory
mkdir -p build
cd build

# Run CMake
cmake ..

# Build the project
make

# Optionally, run the program
./test