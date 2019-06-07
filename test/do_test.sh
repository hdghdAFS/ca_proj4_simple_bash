#!/usr/bin/env bash

# Turn the last line into INSPECTED=true
INSPECTED=false
if [[ $# -eq 1 ]] && ${INSPECTED} ; then
    EXE_LOC=$(pwd)/shell
    echo Running $1
    pushd scripts/$1 > /dev/null
    rm -rf ref ans
    mkdir ref ans
    (cd ref ; bash       ../main.sh > out.txt 2> /dev/null)
    (cd ans ; ${EXE_LOC} ../main.sh > out.txt 2> /dev/null)
    if diff -sr ref ans &> /dev/null ; then
        echo Test passed
    else
        echo Test failed, reference is on the left and yours is on the right:
        diff -r ref ans -y
    fi
    popd > /dev/null

elif [[ $# -eq 0 ]] && ${INSPECTED}  ; then

    echo Building...
    if make &> .build_log ; then
        rm -rf .build_log
    else
        echo Build failed
        cat .build_log
        exit 1
    fi

    TMP_FILE=$(mktemp -d)
    ls scripts > ${TMP_FILE}/tests.txt
    TEST_COUNT=$(wc -l < ${TMP_FILE}/tests.txt)

    echo Collected  tests. Running tests..

    while IFS= read -r test; do
        bash $0 ${test} &> ${TMP_FILE}/${test}.log &
    done < ${TMP_FILE}/tests.txt

    wait

    echo Finished.

    cat ${TMP_FILE}/*.log
else
    cat <<EOF
Usage: $0 [test_name]
Put all files you just unpacked alongside your Makefile and you are
good to go.

I admire your bravery to run random script posted by a potentially
malicious user on internet without any inspection.
EOF
    if [[ "$1" -eq "-h" ]] || [[ "$1" -eq "--help" ]] ; then
        exit 0
    else
        exit 1
    fi
fi
