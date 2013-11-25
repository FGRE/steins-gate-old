#!/bin/bash
export PATH=$MXE/usr/bin
i686-pc-mingw32-gcc src/main.c -I ../libnpengine/include/ -L ../libnpengine/ -lnpengine -o steins-gate.exe -mwindows
