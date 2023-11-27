#!/usr/bin/env bash
if [[ $# -ne 1 ]]
then
    echo "RTFM, desu!"
    echo
    echo "Use command line option --help/-h to get it."
    exit 1
fi

# TODO: add help/usage

for i in $(seq $1)
do

    if [[ 0 -ne $(cmp <(./build/tomboy <./tests/${i}.inp) <(cat tests/${i}.out)) ]]
    then
        echo "*** Test " ${i} " FAILED! ***"
    else
        echo "*** Test " ${i} " PASSED. ***"
    fi
done

exit 0
