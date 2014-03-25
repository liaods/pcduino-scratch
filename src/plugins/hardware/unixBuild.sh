#!/bin/sh
rm -f *.o HardwarePlugin
gcc -fPIC -Wall -c `pkg-config --cflags pangocairo` *.c
gcc -shared `pkg-config --libs pangocairo` -lc *.o -o HardwarePlugin
sudo mv HardwarePlugin /usr/lib/scratch/plugins/so.HardwarePlugin -f
rm -f *.o
