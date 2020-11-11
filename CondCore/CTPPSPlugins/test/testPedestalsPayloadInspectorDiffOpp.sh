#!/bin/bash
# Save current working dir so img can be outputted there later
W_DIR=$(pwd);
# Set SCRAM architecture var
SCRAM_ARCH=slc6_amd64_gcc630;
export SCRAM_ARCH;
source /afs/cern.ch/cms/cmsset_default.sh;
eval `scram run -sh`;
# Go back to original working directory
cd $W_DIR;
# Run get payload data script

####################
# Test Pedestals Hist
####################


getPayloadData.py \
    --plugin pluginPPSPixelGainCalibrations_PayloadInspector \
    --plot   plot_PPSPixelGainCalibrationsPedestalsDiff \
    --tag CTPPSPixelGainCalibrations_v2_offline \
    --tagtwo    CTPPSPixelGainCalibrations_v3_offline \
    --time_type Run \
    --iovstwo    '{"start_iov": "323201", "end_iov": "323201"}' \
    --iovs '{"start_iov": "305582", "end_iov": "305582"}' \
    --db Prod \
    --test;


cp PedestalsComparison.png /eos/user/c/cmora/PCUERJ/CTPPSDB/PedestalsComparison_opp.png

