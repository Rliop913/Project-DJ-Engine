#!/bin/bash
doxygen ./Doxyfile
sphinx-build -b html ./document_sources docs
