#ifndef CondFormats_CTPPSReadoutObjects_PPSRPEfficiencyMaps_h
#define CondFormats_CTPPSReadoutObjects_PPSRPEfficiencyMaps_h

#include "CondFormats/CTPPSReadoutObjects/interface/PPSRPEfficiencyMap.h"
#include "TH2D.h"
#include <map>
#include <vector>


class PPSRPEfficiencyMaps{
 public:
  //map detid to PPSRPEfficiencyMap object (table)
  typedef std::map<uint32_t,PPSRPEfficiencyMap> RPEffMap;

  PPSRPEfficiencyMaps(){}
  virtual ~PPSRPEfficiencyMaps(){}
  void setRPEfficiency(const uint32_t & DetId, const PPSRPEfficiencyMap & efficiency);
  void setRPEfficiency(const uint32_t & DetId, const TH2D & effmaphisto );
  void setRPEfficiencies(const RPEffMap & efficiencies);
  const RPEffMap & RPEfficiencies() const {return m_efficiencies;}
  const PPSRPEfficiencyMap & RPEfficiency(const uint32_t & detid) const;
  
 private:
  RPEffMap m_efficiencies;

  COND_SERIALIZABLE;
}

#endif
