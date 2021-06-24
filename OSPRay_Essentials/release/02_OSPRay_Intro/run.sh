#!/bin/bash
source /opt/intel/inteloneapi/embree/latest/env/vars.sh
source /opt/intel/inteloneapi/openvkl/latest/env/vars.sh
source /opt/intel/inteloneapi/rkcommon/latest/env/vars.sh
source /opt/intel/inteloneapi/ospray/latest/env/vars.sh
/bin/echo "##" $(whoami) is running O2_OSPRay_Intro
bin/ospTutorial

