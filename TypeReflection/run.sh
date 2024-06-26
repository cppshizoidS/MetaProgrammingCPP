#!/bin/bash

# Set default compiler to clang++ if COMPILER is not set
COMPILER=${COMPILER:-clang++}

# Check which compiler to use and set the appropriate flags
if [ "$COMPILER" = "clang++" ]; then
    $COMPILER -Wall -o test main.cpp -std=c++23 && ./test
elif [ "$COMPILER" = "g++" ]; then
    $COMPILER -Wall -o test main.cpp -std=c++23 -Wno-non-template-friend && ./test
else
    echo "Unsupported compiler: $COMPILER"
    exit 1
fi
