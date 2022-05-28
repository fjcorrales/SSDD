#!/bin/sh


cd bin
PROG=tests/$1
test $# -eq 0 || test ! -f $PROG.class && { echo "Debe especificar el nombre del programa de test a ejecutar y sus argumentos" >&2; exit 1; }
shift

set -x
java -Djava.rmi.server.useCodebaseOnly=false -Djava.security.policy=../permisos -cp .:../interfaces.jar $PROG $*

