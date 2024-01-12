#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path to install>"
    exit 1
fi

DEST=$1
DEST_DIR=$DEST/tk-alloc
DEST_SOURCE=$DEST_DIR/source-code

if [ -d "$DEST_DIR" ]; then
    echo "Library folder already exists under destination: $DEST"
    while true; do
        read -p "Do you want to build anyway? Folder will be removed! (y/n) " user_input
        case $user_input in
            [Nn]* ) echo "Exiting script."; exit 0;;
            [Yy]* ) rm -rf $DEST/tk-alloc; break;;
            * ) echo "Please answer y (yes) or n (no).";;
        esac
    done
fi

echo 
    while true; do
        read -p "Do you want to install all necessary packages for the lib? (y/n) " user_input
        case $user_input in
            [Yy]* ) sudo ./install_env.sh; break;;
            [Nn]* ) echo "Still can do it by running: sudo ./install_env.sh"; break;;
            * ) echo "Please answer y (yes) or n (no).";;
        esac
    done

make regression
make clean

mkdir -p $DEST_DIR
mkdir -p $DEST_SOURCE

cp -r app $DEST_SOURCE
cp -r e2eTests $DEST_SOURCE
cp -r test $DEST_SOURCE
cp -r include $DEST_SOURCE
cp -r src $DEST_SOURCE
cp -r *.sh $DEST_SOURCE
cp -r makefile $DEST_SOURCE
cp -r README.md $DEST_SOURCE

cd $DEST_SOURCE
make install
cp -r include lib
mv lib ..

echo "Fininshed successfully."