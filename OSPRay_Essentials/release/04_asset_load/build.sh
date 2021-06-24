#!/bin/bash
#source /opt/intel/inteloneapi/embree/latest/env/vars.sh
#source /opt/intel/inteloneapi/openvkl/latest/env/vars.sh
#source /opt/intel/inteloneapi/rkcommon/latest/env/vars.sh
#source /opt/intel/inteloneapi/ospray/latest/env/vars.sh
#TBBROOT=/opt/intel/inteloneapi/rkcommon/latest
#TBBINCLUDEDIR=$(dirname CMakeLists.txt)/../../third-party
#CMAKE_MODULE_PATH=/opt/intel/inteloneapi/rkcommon/latest/lib/cmake/rkcommon-1.4.2

/bin/echo "##" $(whoami) is building 04_asset_load
TOOLSDIR=$(dirname CMakeLists.txt)/../../tools
source /opt/intel/inteloneapi/setvars.sh
[ ! -d build ] && mkdir build
cd build
rm -rf *
test_cmake=$(cmake --version | grep "3.10.2")
if [ -z "$test_cmake" ] ; then
    echo $(cmake --version)
    # cmake .. -DTBB_ROOT=$TBBROOT -DTBB_INCLUDE_DIR=$TBBINCLUDEDIR -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH
    cmake ..
    make -j8
else  
    echo $($TOOLSDIR/cmake/bin/cmake --version)
    # $TOOLSDIR/cmake/bin/cmake .. -DTBB_ROOT=$TBBROOT -DTBB_INCLUDE_DIR=$TBBINCLUDEDIR -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH
    $TOOLSDIR/cmake/bin/cmake ..
    make -j8
fi

