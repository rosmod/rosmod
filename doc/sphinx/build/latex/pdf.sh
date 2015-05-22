#!/usr/bin/env bash
# Build ROSMOD.pdf

pdflatex ROSMOD.tex
makeindex ROSMOD
pdflatex ROSMOD.tex
