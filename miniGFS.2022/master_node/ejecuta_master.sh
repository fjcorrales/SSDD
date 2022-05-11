#!/bin/sh

set -x

cd bin
java -Djava.rmi.server.codebase=file:$PWD/ -Djava.security.policy=../permisos -cp .:../interfaces.jar master.MasterSrv $*

