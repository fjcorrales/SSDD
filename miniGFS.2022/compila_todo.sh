#!/bin/sh

cd `dirname $0`
for dir in interfaces master_node data_node client_node
do
    test -d $dir || continue
    echo compilando $dir
    cd $dir
    ./compila.sh
    test $? -eq 0 || exit 1
    cd ..
    echo
    echo
done

