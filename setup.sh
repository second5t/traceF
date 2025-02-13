#!/bin/bash

# Run commands silently unless an error occurs
{
    # Download Intel Pin Tool
    wget -q https://software.intel.com/sites/landingpage/pintool/downloads/pin-3.28-98749-g6643ecee5-gcc-linux.tar.gz
    
    # Extract the downloaded archive
    tar -xvf pin-3.28-98749-g6643ecee5-gcc-linux.tar.gz > /dev/null
    
    # Rename the extracted folder
    mv pin-3.28-98749-g6643ecee5-gcc-linux pin
    
    # Remove the original archive
    rm -r pin-3.28-98749-g6643ecee5-gcc-linux.tar.gz

    # Set compiler environment variables
    export CC=gcc
    export CXX=g++
    
    # Build the project
    make > /dev/null
} || {
    # Print error message in red and exit if any command fails
    echo -e "\e[31m[ERROR] Installation failed. Please check the logs.\e[0m"
    exit 1
}

# Print success message in green
echo -e "\e[32m[INFO] Setup completed successfully!\e[0m"

# Print usage instructions in green
echo -e "\e[32mUsage:\e[0m"
echo -e "\e[32m./pin/pin -t obj-intel64/traceF.so -- <program> <arg1> <arg2> ... <argn>\e[0m"

