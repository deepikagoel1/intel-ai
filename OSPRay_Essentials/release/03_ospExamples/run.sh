#!/bin/bash
source /opt/intel/inteloneapi/embree/latest/env/vars.sh
source /opt/intel/inteloneapi/openvkl/latest/env/vars.sh
source /opt/intel/inteloneapi/rkcommon/latest/env/vars.sh
source /opt/intel/inteloneapi/ospray/latest/env/vars.sh

/bin/echo "##" $(whoami) is running 03_ospExamples
bin/ospExamples $1 $2
