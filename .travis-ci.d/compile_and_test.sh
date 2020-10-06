#!/bin/bash

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/current/CI_${COMPILER}
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -GNinja -DCMAKE_CXX_FLAGS="-Werror -fdiagnostics-color=always" .. && \
ninja  -k 0 && \
ninja install 
#&& \
#ctest --output-on-failure
