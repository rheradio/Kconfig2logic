#!/bin/bash
# $1 -> name of the system
# $2 -> number of generated tests

if [ x$KCONF == x ]
then
echo variable KCONF not set
exit -1;
fi

cd $KCONF/../../kaestner
for i in `seq $2`; do
    printf "Creating instance number %5d for %s\n" $i $1
#echo Creating instance number $i for $1
    if ! randomconf $1 >logfile 2>&1; then
        cat logfile
        echo conf execution failed
        exit
    fi
    grep "=y" .config | sed 's/=y//;s/^CONFIG_//' >positive
    grep "=\"\"" .config | sed 's/=\"\"//;s/^CONFIG_//' >>positive
    #grep "is not set" .config | awk '{ print "not "$2}' | sed 's/=y//;s/CONFIG_//' >negated
    while read line; do
    for word in $line; do
        if ! grep $word positive >/dev/null 2>&1; then
            echo "not $word" >>negated
        fi
        done
    done<$KCONF/orderings/$1.var
    cat positive negated $KCONF/orderings/$1.exp >$1.$i.exp 2>/dev/null
    if ! $KCONF/Logic2BDD $EXTRA -min-nodes 10000 -base $1.$i  -cudd -constraint-reorder  minspan  $KCONF/orderings/$1.var $1.$i.exp >>logfile 2>&1; then
       cat logfile
       exit -1
    fi

    solutions=`$KCONF/counter $1.$i`
    if [ x$solutions != x1 ]; then
        cat logfile
        echo The test failed, solutions=$solutions
        exit -1
    fi
rm -f  positive negated logfile $1.$i.data $1.$i.reorder $1.$i.exp $1.$i.dddmp 2>/dev/null
done
echo All test passed!!
