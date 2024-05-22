#!/bin/bash

if test -f muc-universe.template; then
    mkdir build
    cp muc-universe.template build/muc-universe.h++
    amalgamate/amalgamate -v -o build -I include -- build/muc-universe.h++
    cp build/muc-amalgamated/muc.h++ include/muc-universe
fi
