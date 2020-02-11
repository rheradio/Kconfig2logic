#!/bin/bash
# $1 -> name of the system
# $2 -> number of generated tests

POSSIBILITIES="Config.in Config.src Kconfig config/Config.in config/Config.src src/Config.in src/Config.src src/Kconfig extra/Configs/Config.in"
for ONE in $POSSIBILITIES; do
    if [ -f systems/$1/$ONE ]; then
        FILE=$ONE
    fi
done
if [ x$FILE == x ]; then
    echo Could not find a Kconfig file
    exit
fi
cd systems/$1
for i in `seq $2`; do
    printf "Creating instance number %5d for %s " $i $1
    if ! BR2_EXTERNAL=support/dummy-external ../../code/bin/randomconf $FILE >logfile 2>&1; then
        cat logfile
        echo conf execution failed
        exit
    fi
    cat configuration ../../translations/KconfigReader/$1.exp >$1.$i.exp
    ../../code/bin/extractVariables $1.$i.exp >$1.$i.var
    if ! ../../code/bin/Logic2BDD $EXTRA -min-nodes 10000 -base $1.$i  -cudd -constraint-reorder  minspan  $1.$i.var $1.$i.exp  >>logfile 2>&1; then
       cat logfile
       exit
    fi

    solutions=`../../code/bin/counter $1.$i`
    if [ x$solutions != x1 ]; then
        echo " "The test failed, solutions=$solutions
        let "wrong+=1"
    else 
        printf "\n"
        let "right+=1"
    fi
    
rm -f  positive negated logfile $1.$i.data $1.$i.reorder $1.$i.var $1.$i.exp $1.$i.dddmp 2>/dev/null
done
echo right $right" " wrong $wrong
