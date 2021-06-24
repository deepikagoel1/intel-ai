#!/bin/bash
TOOLSDIR=$(dirname CMakeLists.txt)/../../tools
source /opt/intel/inteloneapi/setvars.sh
/bin/echo "##" $(whoami) is compiling O2_OSPRay_Intro
[ ! -d build ] && mkdir build
cd build
rm -rf *
test_cmake=$(cmake --version | grep "3.10.2")
if [ -z "$test_cmake" ] ; then
    echo $(cmake --version)
    cmake ..
    make -j8
else  
    echo $($TOOLSDIR/cmake/bin/cmake --version)
    $TOOLSDIR/cmake/bin/cmake ..
    make -j8
fi
