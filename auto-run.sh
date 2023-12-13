#!/bin/bash

# Compile
gcc -std=c11 -o changeme changeme.c -lm

# Check if compilation was successful
if [ $? -eq 0 ]; then
    clear
    echo "Compilation successful."
    echo -e "Starting the program...\n"
    ./hw7
else
    echo "Compilation failed."
fi

# The script ends here, and the user will have control in the shell
