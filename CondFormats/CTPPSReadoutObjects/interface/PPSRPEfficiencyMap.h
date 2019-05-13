#ifndef CondFormats_CTPPSReadoutObjects_PPSRPEfficiencyMap_h
#define CondFormats_CTPPSReadoutObjects_PPSRPEfficiencyMap_h
// -*- C++ -*-
//
// Package:    CTPPSReadoutObjects
// Class:      PPSRPEfficiencyMap
// 
/**
 class CTPPSRPEfficiencyMap CTPPSRPEfficiencyMap.h CondFormats/CTPPSRedoutObjects/src/PPSRPEfficiencyMap.cc

 Description: Store  index in x, index in y, efficiency table for each PPS roman pot

 Author: Clemencia Mora Herrera clemencia.mora.herrera@cern.ch

**/
//// 

#include "CondFormats/Serialization/interface/Serializable.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <vector>
#include "TH2D.h"
#include <string>
#include <memory>

class PPSRPEfficiencyMap {
 friend class PPSRPEfficiencyMaps;

 public:

 struct effentry{
   uint32_t ix;
   uint32_t iy;
   double xcenter;
   double ycenter;
   double efficiency;
   COND_SERIALIZABLE;
 };

 //Constructors
 PPSRPEfficiencyMap(){}
 PPSRPEfficiencyMap(const uint32_t & nxbins, const uint32_t & nybins);
 PPSRPEfficiencyMap(const TH2D & efficiencyhisto );
 PPSRPEfficiencyMap(const std::string &filename , const std::string &histopath);
 ~PPSRPEfficiencyMap(){}
 void FillPPSRPEfficiencyMap(const TH2D & efficiencyhisto);
 void setEffEntry(const uint32_t& xb, const uint32_t& yb, const double & eff);
 void setnTracks(int nt);
 double efficiency(const double& x, const double& y)const;
 double efficiency(const uint32_t& xbin, const uint32_t& ybin) const;
 double efficiency(const uint32_t & nbin) const;
 int nTracks() const {return m_nTracks;}
 int nBins()const{return m_nBins;}
 std::vector<effentry> efficiencyTable() const{return m_efficiencytable;}
 void initialize(){}


 private:
 std::vector<effentry> m_efficiencytable;
 int m_nTracks;
 int m_nBins;
 int m_nBinsX;
 int m_nBinsY;
 double m_xLow;
 double m_yLow;
 double m_xWidth;
 double m_yWidth;

 COND_SERIALIZABLE;
};

#endif
