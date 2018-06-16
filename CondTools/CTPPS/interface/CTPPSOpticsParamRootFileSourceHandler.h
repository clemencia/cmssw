/*
 *
 * Created by Clemencia Mora Herrera clemencia.mora.herrera@cern.ch
 * based on CondTools/DQM/interface/DQMReferenceHistogramRootFileSourceHandler.h
 *
 * June 2018
 */

#ifndef CTPPSOpticsParamRootFileSourceHandler_h
#define CTPPSOpticsParamRootFileSourceHandler_h

#include <string>

#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondFormats/Common/interface/FileBlob.h"

//namespace popcon {
class CTPPSOpticsParamRootFileSourceHandler : public popcon::PopConSourceHandler<FileBlob> {
 public:
  CTPPSOpticsParamRootFileSourceHandler(const  edm::ParameterSet  & pset);
  ~CTPPSOpticsParamRootFileSourceHandler() override;
  void getNewObjects()  override;
  std::string id() const override;
 private:
  std::string m_name;
  std::string m_file;
  bool m_zip;
  unsigned long long m_since;
  bool m_debugMode;
};
//}

#endif
