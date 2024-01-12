#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path to install>"
    exit 1
fi

DEST_DIR=$1
DEST_LIB=$DEST_DIR/lib

make regression
make clean
mkdir -p $DEST_LIB
cp -r include $DEST_LIB
mkdir -p $DEST_DIR/source
cp -r . $DEST_DIR/source
