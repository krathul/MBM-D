#!/bin/bash

# Check if file exists
if [ ! -f "$1" ]; then
    echo "File not found!"
    exit 1
fi

# Read file and find the maximum integer
max=$(cat "$1" | tr ' ' '\n' | sort -nr | head -n1)

echo "Overall largest integer: $max"

