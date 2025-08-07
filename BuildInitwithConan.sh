#!/bin/bash
conan install $1 --output-folder=conan_cmakes --build=missing --settings compiler.cppstd=20 --settings compiler.runtime=$2 --settings build_type=$3