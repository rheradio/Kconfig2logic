#!/bin/bash
if [ x$KCONF = x ]
  then
    echo variable KCONF not set
    exit -1;
fi
SYSTEMS="axtls  buildroot  busybox  coreboot  embtoolkit  fiasco  freetz  toybox  uClibc"
for i in $SYSTEMS
do
  echo Cleaning system $i
  cd $KCONF/systems/$i
  rm configuration* *.reorder *data *.exp logfile* 2>/dev/null
done
wait
