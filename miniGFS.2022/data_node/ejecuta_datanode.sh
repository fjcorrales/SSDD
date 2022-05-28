#!/bin/sh

set -x

cd bin
java -Djava.security.policy=../permisos -cp .:../interfaces.jar datanode/DataNodeSrv $*

