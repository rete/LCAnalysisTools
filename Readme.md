# LCAnalysisTools 

Collection of various tools for physics analysis with Linear Colliders

[![Build Status](https://travis-ci.com/rete/LCAnalysisTools.svg?branch=master)](https://travis-ci.com/rete/LCAnalysisTools)

## Installation

```shell
# Download it
git clone https://github.com/rete/LCAnalysisTools.git
cd LCAnalysisTools
# seclect a version (if any)
git tag
git checkout <version>
# source iLCSoft, e.g:
source /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02/init_ilcsoft.sh
# compile the software
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake [-DOPTION=ON/OFF] ..
make install
```

Options can be given to CMake:

- INSTALL_DOC (ON/OFF): to generate and install C++ API documentation using Doxygen

## Usage

See doc/Readme.md for usage documentation

## Documentation

The examples provided in source/examples serve as documentation in addition to the C++ API documentation generated when configuring the software with `-DINSTALL_DOC=ON`.

## Copyright and Licence

TimingAnalaysis is distributed under the [BSD-3-Clause licence](http://opensource.org/licenses/BSD-3-Clause).
