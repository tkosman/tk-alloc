#!/bin/bash

detect_distro() {
    if [ -f /etc/debian_version ]; then
        PKG_UPDATE="apt-get update -y"
        PKG_INSTALL="apt-get install -y"
    elif [ -f /etc/redhat-release ]; then
        PKG_UPDATE="yum update -y"
        PKG_INSTALL="yum install -y"
    elif [ -f /etc/arch-release ]; then
        PKG_UPDATE="pacman --noconfirm -Sy"
        PKG_INSTALL="pacman --noconfirm -S"
    elif [ -f /etc/SuSE-release ]; then
        PKG_UPDATE="zypper refresh"
        PKG_INSTALL="zypper install"
    else
        echo "Unsupported Linux distribution."
        exit 1
    fi
}

detect_distro
export PKG_UPDATE PKG_INSTALL
