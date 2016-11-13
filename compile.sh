#!/bin/sh

make clean
qmake -project && qmake && make