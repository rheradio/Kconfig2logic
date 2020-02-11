#!/bin/bash 
SYSTEMS="axtls  buildroot  busybox  coreboot  embtoolkit  fiasco  freetz  toybox  uClibc"
for i in $SYSTEMS
do
  echo Cleaning system $i
  cd systems/$i
  rm configuration* *.reorder *data *.exp logfile* 2>/dev/null
  cd ../..
done
