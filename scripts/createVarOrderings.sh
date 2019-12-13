#!/bin/bash
if [ x$KCONF = x ]
  then
    echo variable KCONF not set
    exit -1;
fi
SYSTEMS="axtls  buildroot  busybox  coreboot  embtoolkit  fiasco  freetz  toybox  uClibc"
for i in $SYSTEMS
do
  $KCONF/scripts/createOneOrdering.sh $i &
done
wait
