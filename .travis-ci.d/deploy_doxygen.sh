#!/bin/bash

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/current/CI_${COMPILER}
source $ILCSOFT/init_ilcsoft.sh

# Build the doc
cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -GNinja -DINSTALL_DOC=ON .. && \
ninja  -k 0 && \
ninja doxygen || exit 1;

# Push the doc
git checkout doc && \
cp -r docbuild/html/* .. && \
cd .. && \
rm -rf build && \
ls -lr && \
git add . && \
git -c user.name='travis' -c user.email='travis' commit -m "Documentation update `date`" && \
git remote add docremote https://rete:$GITHUB_ACCESS_TOKEN@github.com/rete/LCAnalysisTools.git && \
git push -fq docremote doc

#&& \
#ctest --output-on-failure
