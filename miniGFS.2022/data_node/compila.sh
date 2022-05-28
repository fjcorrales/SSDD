#!/bin/sh
  
set -x

cd src
javac -cp ../interfaces.jar -d ../bin datanode/DataNodeImpl*.java datanode/DataNodeSrv.java datanode/LockManager.java

