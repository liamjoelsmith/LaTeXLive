#!/bin/bash

pdflatex -output-directory=output minimal.tex

mv output/minimal.pdf ..