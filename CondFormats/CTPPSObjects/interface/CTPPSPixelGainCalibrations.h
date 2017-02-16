#ifndef CondFormats_CTPPSObjects_CTPPSPixelGainCalibrations_h
#define CondFormats_CTPPSObjects_CTPPSPixelGainCalibrations_h

#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibration.h"
#include <map>
#include <vector>

using namespace std;

class CTPPSPixelGainCalibrations{
 public:
  typedef map<uint32_t,CTPPSPixelGainCalibration> calibmap;

  CTPPSPixelGainCalibrations(){}
  virtual ~CTPPSPixelGainCalibrations(){}

  void setGainsCalibrations(const uint32_t& DetId,const CTPPSPixelGainCalibration & PixGains);
  void setGainsCalibrations(const uint32_t& DetId,const vector<float>& peds, vector<float>& gains);
  void setGainsCalibrations(const calibmap & PixGainsCalibs);
  void setGainsCalibrations(const vector<uint32_t>& detidlist, const vector<vector<float>>& peds, const vector<vector<float>>& gains);

  const calibmap & getCalibmap()const { return m_calibrations;}

  CTPPSPixelGainCalibration getGainsCalibration(uint32_t detid) const;
  CTPPSPixelGainCalibration & getGainsCalibration(uint32_t detid);

  
  int size() const {return m_calibrations.size();}

 private:
  calibmap m_calibrations;

  COND_SERIALIZABLE;
}
