#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Update package list and install GNU Readline library
echo "Installing GNU Readline library..."
sudo apt update
sudo apt install -y libreadline-dev

# Build SYSCPPCPGenerator
echo "Building SYSCPPCPGenerator..."
cd SYSCPPCPcodeGenrtators/SYSCPPCPgenerator/
make

# Build SmallSQLgenerator
echo "Building SmallSQLgenerator..."
cd ../SmallSQLgenerator/
make

# Run SYSCPPCPGenerator
echo "Running SYSCPPCPGenerator..."
cd ../debug/
./SYSCPPCPGenerator

# Run SmallSQLgenerator
echo "Running SmallSQLgenerator..."
./SmallSQLgenerator

# Build  test
cd ../../SYSCPPCPtest/GenTestData
make
cd ../QuickStart
make
echo "Build and execution complete."

# Run SmallSQLd with syscppcp.dat
echo "Running SmallSQLd with syscppcp.dat..."
cd ../../SmallSQL/debug/
./SmallSQLd ../../syscppcp.dat