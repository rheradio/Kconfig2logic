#!/bin/bash
SYSTEMS="axtls  buildroot  busybox  coreboot  embtoolkit  fiasco  freetz  toybox  uClibc"
for i in $SYSTEMS
do
  $KCONF/scripts/createOneOrdering.sh $i &
done
wait
