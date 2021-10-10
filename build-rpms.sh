#!/bin/bash

##########
# Configuration
##########

export APP_NAME=grep "Name" $(find -name "*.spec" | head -1) | awk '{print $2}'
export SFOS_VER=4.2.0.21

##########
# Paths
##########

export ORIG_PATH=$PATH
export PROJECT=$PWD
export PRO_FILE=$PROJECT/$APP_NAME.pro
export SHADOW=$PROJECT/shadow
export RPM_DEST_DIR=$PROJECT/RPMS

##########
# Build function. Ran once per architecture
##########

function build() {
  export PATH="$HOME/SailfishOS/bin:$HOME/mersdk/targets/Sailfish-$SFOS_VER-$ARCH.default/usr/lib/qt5/bin:$HOME/.config/SailfishSDK/libsfdk/build-target-tools/Sailfish SDK Build Engine/SailfishOS-$SFOS_VER-$ARCH.default:$ORIG_PATH"
  export SFDK_OPTIONS="-c target=SailfishOS-$SFOS_VER-$ARCH"
  cd $PROJECT
  mkdir $SHADOW
  cd $SHADOW
  mkdir $ARCH
  cd $ARCH
  if [ ! -f Makefile ]
  then
    sfdk qmake $PROJECT -recursive
  fi
  sfdk make
  sfdk package
  perl-rename -v 's/\+.{29}//g' RPMS/*.rpm
  cp -f -v RPMS/*.rpm $RPM_DEST_DIR/
  cd $PROJECT
}

##########
# Build the packages
##########

mkdir $RPM_DEST_DIR

export ARCH=aarch64
build

export ARCH=armv7hl
build

export ARCH=i486
build

##########
# Run the validator only once
##########

$HOME/SailfishOS/bin/sfdk engine exec rpmvalidation -t SailfishOS-$SFOS_VER-$ARCH.default $(ls RPMS/*$ARCH.rpm | head -1)

echo -e "\nDone!.\n"
echo "For clean build, remove '$SHADOW' folder."
echo "For qmake, remove '$SHADOW/*/Makefile'"
