#!/bin/bash
if [ x$KCONF = x ]
  then
    echo variable KCONF not set
    exit -1;
fi

cd $KCONF/translations/Kconfig2Logic
if [ "$1" = "buildroot" ]; then
    EXTRA=$EXTRA" BR2_EXTERNAL=support/dummy-external"
fi
  printf "Processing %30s\n" $1
  $KCONF/code/bin/Kconfig2Logic  $EXTRA $KCONF/systems/$1                 >$1.logfile1 2>&1 



