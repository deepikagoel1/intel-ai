#!/bin/bash
source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
/bin/echo "##" $(whoami) is compiling DPCPP_Essentials Module4 -- DPCPP Sub Groups - 5 of 6 sub_group_broadcast.cpp
dpcpp lab/sub_group_broadcast.cpp 
if [ $? -eq 0 ]; then ./a.out; fi


