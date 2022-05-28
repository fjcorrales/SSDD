#!/bin/sh
  
set -x

cd src
javac -cp .:../interfaces.jar -d ../bin tests/*.java client/*.java


