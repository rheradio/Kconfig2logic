#!/bin/bash
# $1 -> name of the system
# $2 -> number of generated tests
# $3 -> number of simultaneous tests
function generateAndTest() {
    printf "Creating instance number %5d for %s\n" $1 $2
    if ! $KCONF/code/bin/randomconf $1 $3 >logfile.$1 2>&1; then
        cat logfile.$1
        echo randomconf execution $1 failed
        kill $$
        exit
    fi
    cat configuration.$1 $KCONF/translations/Kconfig2Logic/$2.exp >$2.$1.exp
    if ! $KCONF/code/bin/Logic2BDD $EXTRA -base $2.$1 -min-nodes 1000000  -cudd -constraint-reorder minspan    $KCONF/translations/Kconfig2Logic/$2.var $2.$1.exp  >>logfile.$1 2>&1; then
      cat logfile.$1
      kill $$
      exit
    fi

  solutions=`$KCONF/code/bin/counter $2.$1`
  if [ x$solutions != x1 ]; then
    cat logfile.$1
    echo The test failed, solutions=$solutions in file $2.$1
    kill $$
    exit
  fi
  rm -f configuration.$1 logfile.$1 $2.$1.data $2.$1.reorder $2.$1.exp $2.$1.dddmp 2>/dev/null

}
export -f generateAndTest

if [ x$KCONF == x ]
then
echo variable KCONF not set
exit -1;
fi
POSSIBILITIES="Config.in Config.src Kconfig config/Config.in config/Config.src src/Config.in src/Config.src src/Kconfig extra/Configs/Config.in"
for ONE in $POSSIBILITIES; do
    if [ -f $KCONF/systems/$1/$ONE ]; then
        FILE=$KCONF/systems/$1/$ONE
    fi
done
if [ x$FILE == x ]; then
    echo Could not find a Kconfig file
    exit
fi
cd $KCONF/systems/$1
seq $2 | xargs -n 1 -P $3 -I {} bash -c 'generateAndTest "$@"' _ {} $1 $FILE
