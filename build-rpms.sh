#!/bin/bash
set -e

# For clean build, remove $SHADOW folder
# To trigger qmake, remove $SHADOW/*/Makefile

##########
# Configuration
##########

export SFOS_VER=5.0.0.62
export SDK_DIR="$HOME/SailfishOS"

##########
# Paths and versions
##########

export PROJECT=$PWD
export SHADOW=$PROJECT/shadow
export RPM_DEST_DIR=$PROJECT/RPMS

export APP_NAME=$(grep "Name" $(find . -regextype egrep -regex "\.\/rpm\/[a-z0-9_-]*\.(spec)" -print -quit) | awk '{print $2}')
export APP_VER=$(grep "Version" $(find . -regextype egrep -regex "\.\/rpm\/[a-z0-9_-]*\.(spec)" -print -quit) | awk '{print $2}')
export APP_REL=$(grep "Release" $(find . -regextype egrep -regex "\.\/rpm\/[a-z0-9_-]*\.(spec)" -print -quit) | awk '{print $2}')

export PRO_FILE=$PROJECT/$APP_NAME.pro

##########
# Build function. Ran once per architecture
##########

function build() {
  if [ ! -d "$SDK_DIR/mersdk/targets/SailfishOS-$SFOS_VER-$ARCH.default" ]; then
    echo Invalid build target.
    echo $SDK_DIR/mersdk/targets/SailfishOS-$SFOS_VER-$ARCH.default
    exit 1
  fi

  local PATH="$SDK_DIR/bin:$SDK_DIR/mersdk/targets/SailfishOS-$SFOS_VER-$ARCH.default/usr/lib/qt5/bin:$HOME/.config/SailfishSDK/libsfdk/build-target-tools/Sailfish SDK Build Engine/SailfishOS-$SFOS_VER-$ARCH.default:$PATH"

  pushd $PROJECT
  [ -d "$SHADOW/$ARCH" ] || mkdir -p "$SHADOW/$ARCH"
  pushd "$SHADOW/$ARCH"

  sfdk config --session --push no-fix-version
  sfdk config --session --push target SailfishOS-$SFOS_VER-$ARCH
  sfdk config --session --push-mask output-prefix

  [ -f "Makefile" ] || sfdk qmake $PROJECT -recursive
  sfdk make
  sfdk package

  sfdk config --session --drop no-fix-version
  sfdk config --session --drop target
  sfdk config --session --drop output-prefix

  mv -f -v RPMS/*.rpm $RPM_DEST_DIR/

  popd
  popd
}

##########
# Build the packages
##########

[ -d "$RPM_DEST_DIR" ] || mkdir $RPM_DEST_DIR

if [ $(grep "^BuildArch:\W*noarch$" $(find . -regextype egrep -regex "\.\/rpm\/[a-z0-9_-]*\.(yaml|spec)" -print) | wc -l) -gt 0 ]
then
  RPM_NOARCH=1
  RPM_ARCH=noarch
else
  RPM_NOARCH=0
  RPM_ARCH=aarch64
fi

for ARCH in i486 armv7hl aarch64; do
  build
  # If noarch, only build i486
  [ $RPM_NOARCH -eq 0 ] || break
done

export RPM_FILE="RPMS/$APP_NAME-$APP_VER-$APP_REL.$RPM_ARCH.rpm"

if [ -f "$RPM_FILE" ]; then
  "$SDK_DIR/bin/sfdk" engine exec rpmvalidation -t "SailfishOS-$SFOS_VER-$ARCH.default" "$RPM_FILE"
else
  echo "RPM package not found, not validating."
fi
