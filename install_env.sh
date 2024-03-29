#!/bin/bash

if [ "$(id -u)" != "0" ]; then
   echo "This script requires sudo" 1>&2
   exit 1
fi

source detect_distribution.sh

# Package update
$PKG_UPDATE

# Install needed packages
$PKG_INSTALL    gcc \
                make \
                valgrind \
                clang-tidy \
                clang \
                clang-tools

# Install python and packages
$PKG_INSTALL python3
pip install ctype \
            pytest

