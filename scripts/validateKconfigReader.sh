#!/bin/bash
# $1 -> name of the system
# $2 -> number of generated tests

if [ x$KCONF == x ]
then
echo variable KCONF not set
exit -1;
fi
POSSIBILITIES="Config.in Config.src Kconfig config/Config.in config/Config.src src/Config.in src/Config.src src/Kconfig extra/Configs/Config.in"
for ONE in $POSSIBILITIES; do
    #echo Checking $KCONF/systems/$1/$ONE
    if [ -f $KCONF/systems/$1/$ONE ]; then
        FILE=$KCONF/systems/$1/$ONE
        #echo Found in $KCONF/systems/$1/$ONE
    fi
done
if [ x$FILE == x ]; then
    echo Could not find a Kconfig file
    exit
fi
cd $KCONF/systems/$1
for i in `seq $2`; do
    printf "Creating instance number %5d for %s " $i $1
    if ! BR2_EXTERNAL=support/dummy-external $KCONF/code/bin/randomconf $FILE >logfile 2>&1; then
        cat logfile
        echo randomconf execution failed
        exit
    fi
    cat configuration $KCONF/translations/KconfigReader/$1.exp >$1.$i.exp
    $KCONF/code/bin/extractVariables $1.$i.exp >$1.$i.var
    if ! $KCONF/code/bin/Logic2BDD $EXTRA -min-nodes 10000 -base $1.$i  -cudd -constraint-reorder  minspan  $1.$i.var $1.$i.exp  >>logfile 2>&1; then
       cat logfile
       exit
    fi

    solutions=`$KCONF/code/bin/counter $1.$i`
    if [ x$solutions != x1 ]; then
        echo " "The test failed, solutions=$solutions
        let "wrong+=1"
    else 
        printf "\n"
        let "right+=1"
        rm -f  configuration logfile $1.$i.data $1.$i.reorder $1.$i.var $1.$i.exp $1.$i.dddmp 2>/dev/null
    fi
    

done
echo right $right" " wrong results $wrong
