#!/bin/bash

# For clean build, remove $SHADOW folder
# To trigger qmake, remove $SHADOW/*/Makefile

##########
# Configuration
##########

export APP_NAME=$(grep "Name" $(find . -regextype egrep -regex "\.\/rpm\/[a-z0-9_-]*\.(yaml|spec)" -print | head -1) | awk '{print $2}')
export SFOS_VER=4.4.0.58

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
  if [ ! -d "$HOME/SailfishOS/mersdk/targets/SailfishOS-$SFOS_VER-$ARCH.default" ]
  then
    echo Invalid build target.
    echo $HOME/SailfishOS/mersdk/targets/SailfishOS-$SFOS_VER-$ARCH.default
    exit 1
  fi
  export PATH="$HOME/SailfishOS/bin:$HOME/SailfishOS/mersdk/targets/SailfishOS-$SFOS_VER-$ARCH.default/usr/lib/qt5/bin:$HOME/.config/SailfishSDK/libsfdk/build-target-tools/Sailfish SDK Build Engine/SailfishOS-$SFOS_VER-$ARCH.default:$ORIG_PATH"
  export SFDK_OPTIONS="-c target=SailfishOS-$SFOS_VER-$ARCH"
  sfdk config --global --push no-fix-version
  cd $PROJECT
  if [ ! -d "$SHADOW" ]
  then
    mkdir $SHADOW
  fi
  cd $SHADOW
  if [ ! -d "$ARCH" ]
  then
    mkdir $ARCH
  fi
  cd $ARCH
  if [ ! -f "Makefile" ]
  then
    sfdk qmake $PROJECT -recursive
  fi
  sfdk make
  sfdk package
#  perl-rename -v 's/\+.{29}//g' RPMS/*.rpm
  cp -f -v RPMS/*.rpm $RPM_DEST_DIR/
  cd $PROJECT
}

##########
# Build the packages
##########

RPM_NOARCH=0
if [ $(grep "^BuildArch:\W*noarch$" $(find . -regextype egrep -regex "\.\/rpm\/[a-z0-9_-]*\.(yaml|spec)" -print) | wc -l) -gt 0 ]
then
  RPM_NOARCH=1
fi

if [ ! -d "$RPM_DEST_DIR" ]
then
  mkdir $RPM_DEST_DIR
fi

if [ $RPM_NOARCH -eq 0 ]
then
  export ARCH=aarch64
  build

  export ARCH=armv7hl
  build
fi

# Use i486 for noarch build
export ARCH=i486
build

##########
# Run the validator only once
##########

if [ $RPM_NOARCH -gt 0 ]
then
  RPM_ARCH=noarch
else
  RPM_ARCH=$ARCH
fi

export RPM_FILE=$(ls RPMS/*$RPM_ARCH.rpm | head -1)

if [ -f "$RPM_FILE" ]
then
  $HOME/SailfishOS/bin/sfdk engine exec rpmvalidation -t SailfishOS-$SFOS_VER-$ARCH.default $(ls RPMS/*$RPM_ARCH.rpm | head -1)
else
  echo "RPM package not found, not validating."
fi
