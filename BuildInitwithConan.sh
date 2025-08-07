#!/bin/bash
conan install $1 --output-folder=conan_cmakes --build=missing --settings compiler.cppstd=20 --settings build_type=$2 --deployer=direct_deploy