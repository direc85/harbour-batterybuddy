#!/bin/bash
SPEC=rpm/$1.spec
if [[ $(grep "^Version:    $2$" $SPEC | wc -l) = 0 ]]
then
  sed -i "/^Version:    /c\Version:    $2" $SPEC
  touch src/$1.cpp
fi
if [[ $(grep "^Release:    $3$" $SPEC | wc -l) = 0 ]]
then
  sed -i "/^Release:    /c\Release:    $3" $SPEC
  touch src/$1.cpp
fi
echo true
