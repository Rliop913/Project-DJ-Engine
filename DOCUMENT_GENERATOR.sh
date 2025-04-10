#!/bin/bash
doxygen ./Doxyfile
sphinx-build -b html ./source docs