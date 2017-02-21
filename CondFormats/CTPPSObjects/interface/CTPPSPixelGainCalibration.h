#ifndef CondFormats_CTPPSObjects_CTPPSPixelGainCalibration_h
#define CondFormats_CTPPSObjects_CTPPSPixelGainCalibration_h
// -*- C++ -*-
//
// Package:    CTPPSObjects
// Class:      CTPPSPixelGainCalibration
// 
/**\class CTPPSPixelGainCalibration CTPPSPixelGainCalibration.h CondFormats/CTPPSObjects/src/CTPPSPixelGainCalibration.cc

 Description: Gain calibration object for the CTPPS 3D Pixels.  Store gain/pedestal information at pixel granularity

 Implementation:
     <Notes on implementation>
*/
// Loosely based on SiPixelObjects/SiPixelGainCalibration 
// Original Author:  Vincenzo Chiochia
//         Created:  Tue 8 12:31:25 CEST 2007
//         Modified: Evan Friis
// $Id: CTPPSPixelGainCalibration.h,v 1.8 2009/02/10 17:25:42 rougny Exp $
//  Adapted for CTPPS : Clemencia Mora Herrera       November 2016
//

#include "CondFormats/Serialization/interface/Serializable.h"

#include<vector>
#include<map>
#include<iostream>
#include<boost/cstdint.hpp>

class CTPPSPixelGainCalibrations;

class CTPPSPixelGainCalibration {
 friend class CTPPSPixelGainCalibrations;
 public:
  
  struct DetRegistry{ //to index the channels in each sensor 
    uint32_t detid = 0;
    uint32_t ibegin= 0;
    uint32_t iend  = 0;
    uint32_t ncols = 0;
    
    COND_SERIALIZABLE;
  };
  
  
  // Constructors
  CTPPSPixelGainCalibration();
  CTPPSPixelGainCalibration(const uint32_t & detId, const uint32_t & sensorSize, const uint32_t & nCols);
  CTPPSPixelGainCalibration(float minPed, float maxPed, float minGain, float maxGain);
  CTPPSPixelGainCalibration(const uint32_t& detid, const std::vector<float>& peds, const std::vector<float>& gains, 
			    float minPed=0., float maxPed=255., float minGain=0., float maxGain=255.);
  ~CTPPSPixelGainCalibration(){}

  void initialize(){}

  void setGainsPeds(const uint32_t& detId, const std::vector<float>& peds, const std::vector<float>& gains);

  double getGainLow() const { return minGain_; }
  double getGainHigh() const { return maxGain_; }
  double getPedLow() const { return minPed_; }
  double getPedHigh() const { return maxPed_; }

  // Set and get public methods

  void  setDeadPixel(int ipix)  { putData(ipix, -999., 0. ); } // dead flag is pedestal = -999.
  void  setNoisyPixel(int ipix) { putData(ipix, 0., -9999. ); } // noisy flat is gain= -9999.


  void putData(uint32_t ipix, float ped, float gain);

  float getPed   (const int& col, const int& row , bool& isDead, bool& isNoisy) const;
  float getGain  (const int& col, const int& row , bool& isDead, bool& isNoisy) const;
  float getPed   (const uint32_t ipix, bool& isDead, bool& isNoisy)const;
  float getGain  (const uint32_t ipix, bool& isDead, bool& isNoisy)const;

  //get information related to indexes
  uint32_t getDetId () const {return indexes.detid;}
  uint32_t getNCols () const {return indexes.ncols;}
  uint32_t getIBegin() const {return indexes.ibegin;}
  uint32_t getIEnd  () const {return indexes.iend;}

 private:
  void setIndexes(const uint32_t& detId);
  void resetPixData(uint32_t ipix, float ped, float gain);

  std::vector<float> v_pedestals; 
  std::vector<float> v_gains;
  DetRegistry indexes;
  // a single detRegistry w/ detID and collection of indices 
  float  minPed_, maxPed_, minGain_, maxGain_; //not really used

 COND_SERIALIZABLE;
};
    
#endif
