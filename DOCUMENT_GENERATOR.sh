#!/bin/bash
doxygen ./Doxyfile
sphinx-build -b html ./source dox_out