#!/bin/bash

errexit()
{
    echo Error: $1
    exit 1
}


function tools()
{
  g++ --version || errexit "g++ program is missing"
  gcov --version || errexit "gcov program is missing"
  valgrind --version  || errexit "valgrind program is missing"
  doxygen --version  || errexit "doxygen program is missing"
  dot -V  "dot program is missing"
  lscpu
  gcovr --version || errexit "gcovr program is missing"
}

function executecode()
{
  detectors=$(find . -name "*.so" | sed -e 's/\.so//')
  for detector in $detectors
  do
    echo "Starting detector "$detector
    ./prototype2/efu2 -d $detector -s 5
  done
}

rm -fr build
mkdir build

tools
cloc --by-file --xml --out=cloc.xml .
pushd build
cmake -DCOV=y ..        || errexit "cmake failed"
make                    || errexit "make failed"
make VERBOSE=y runtest  || errexit "make runtest failed"
executecode
make coverage           || errexit "make coverage failed"
make valgrind
popd
