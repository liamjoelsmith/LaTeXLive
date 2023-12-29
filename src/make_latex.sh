#!/bin/bash

# Check if a filename was provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

FILENAME="$1"

pdflatex -output-directory=output $FILENAME.tex
