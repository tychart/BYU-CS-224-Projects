#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 [-b] <c_program_file>"
    exit 1
fi

build_only=false

# Check if the first argument is a flag
if [ "$1" == "-b" ]; then
    build_only=true
    shift  # Remove the flag from the argument list
fi

c_file="$1"
executable="${c_file%.c}"

# Compile the C program
#gcc "$c_file" -o "$executable"      # Default
# gcc -g -Wall -DDEBUG "$c_file" -o "$executable"   # For Debugging
gcc -g -Wall "$c_file" -o "$executable"


# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful."

    if [ "$build_only" = false ]; then
        echo "Running the program..."
        echo "----------------------------------------------"
        
        # Run the compiled program
        # "./$executable"     # Default

        # "./$executable" < images/orig-fish.bmp   # Testing (Prints to standard out)

        "./$executable" < images/orig-horses.bmp > testing-face.bmp


        # echo cat images/orig-face.bmp | "./$executable"

        echo "----------------------------------------------"
    else
        echo "Build only. Execution skipped."
    fi
else
    echo "Compilation failed."
fi