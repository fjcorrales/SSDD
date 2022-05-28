#!/bin/sh

set -x
CLASSPATH=.:interfaces.jar exec rmiregistry $*
