#!/bin/bash

if test -f muc-universe.in; then
    mkdir build
    cp muc-universe.in build/muc-universe.h++
    amalgamate/amalgamate -v -o build -I include -- build/muc-universe.h++
    cp build/muc-amalgamated/muc.h++ include/muc-universe
else
    echo "Try execute this script at the top directory!"
fi
