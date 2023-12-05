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

PASSED=0
FAILED=0

for i in $(seq $1)
do
    cmp -s <(./build/tomboy ./tests/${i}.paracl) <(cat tests/${i}.out)
    if [ $? -ne 0 ]
    then
        echo "*** Test " ${i} " FAILED! ***"
        FAILED=$((FAILED+1))
    else
        echo "*** Test " ${i} " PASSED. ***"
        PASSED=$((PASSED+1))
    fi
done

echo "*** ${FAILED} TESTS FAILED! ***"
echo "*** ${PASSED} TESTS PASSED! ***"

exit 0
