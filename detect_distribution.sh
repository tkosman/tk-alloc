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
    elif [ "$(uname)" = "Darwin" ]; then
        # Check if Homebrew is installed
        if ! command -v brew &> /dev/null; then
            echo "Homebrew not found. Installing Homebrew..."
            # Installing Homebrew
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        fi
        PKG_UPDATE="brew update"
        PKG_INSTALL="brew install"
    else
        echo "Unsupported Linux distribution."
        exit 1
    fi
}

detect_distro
export PKG_UPDATE PKG_INSTALL
