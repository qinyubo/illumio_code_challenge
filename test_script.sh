#!/bin/bash

# Compile file
g++ -std=c++11 ./src/nattable.cpp ./src/nat_main.cpp -o nat

# Run test
./nat

# Show OUPUT file
echo ""
echo "Open OUTPUT.txt file: "
echo ""

cat ./OUTPUT.txt

rm nat