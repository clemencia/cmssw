#include "CondCore/ESSources/interface/registration_macros.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSRPPositions.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibration.h"
#include "CondFormats/DataRecord/interface/CTPPSRPPositionsRcd.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelGainCalibrationRcd.h"

REGISTER_PLUGIN(CTPPSRPPositionsRcd,CTPPSRPPositions);
REGISTER_PLUGIN(CTPPSPixelGainCalibrationRcd,CTPPSPixelGainCalibration);
