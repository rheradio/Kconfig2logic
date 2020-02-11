#!/bin/bash
SYSTEMS="axtls  buildroot  busybox  coreboot  embtoolkit  fiasco  freetz  toybox  uClibc"
for i in $SYSTEMS
do
  ./createOneOrdering.sh $i &
done
wait
