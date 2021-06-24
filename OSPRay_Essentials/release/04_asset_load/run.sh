#!/bin/bash
source /opt/intel/oneapi/setvars.sh &>/dev/null

/bin/echo "##" $(whoami) is running 04_asset_load
bin/asset_loader $1 $2 $3
