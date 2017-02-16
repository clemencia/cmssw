#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibrations.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <iostream>

CTPPSPixelGainCalibrations::setGainCalibrations(const uint32_t detid, const CTPPSPixelGainCalibration & PixGains){
  m_calibrations[detid]=PixGains;
}

CTPPSPixelGainCalibrations::setGainCalibrations(const uint32_t detid, vector<float> peds, vector<float> gains){
  m_calibrations[detid]=  CTPPSPixelGainCalibration(detid,peds,gains);
}
