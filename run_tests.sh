#!/usr/bin/env bash
if [[ $# -ne 1 ]]
then
    echo "Usage: $0 TESTS_NUMBER"
    echo
    echo "RTFM, desu!"
    echo "Use command line option --help/-h to get it."
    exit 1
fi

# TODO: add help/usage

for i in $(seq $1)
do
    cmp -s <(./build/tomboy ./tests/${i}.paracl) <(cat tests/${i}.out)
    if [ $? -ne 0 ]
    then
        echo "*** Test " ${i} " FAILED! ***"
    else
        echo "*** Test " ${i} " PASSED. ***"
    fi
done

exit 0
