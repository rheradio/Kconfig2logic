#!/bin/bash
cd translations/Kconfig2Logic
if [ "$1" = "buildroot" ]; then
    EXTRA=$EXTRA" BR2_EXTERNAL=support/dummy-external"
fi
  printf "Processing %30s\n" $1
  ../../code/bin/Kconfig2Logic  $EXTRA ../../systems/$1                 >$1.logfile1 2>&1 



