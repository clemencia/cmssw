// system include files
#include <memory>
#include <iostream>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondCore/CondDB/interface/Time.h"

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsData.h"
#include "CondFormats/DataRecord/interface/CTPPSRPAlignmentCorrectionsDataRcd.h"

using namespace std;

class CTPPSRPAlignmentInfoReader : public edm::EDAnalyzer {
public:
  
  cond::Time_t iov_;
  
  explicit  CTPPSRPAlignmentInfoReader(edm::ParameterSet const& iConfig):
    iov_(iConfig.getParameter<unsigned long long>("iov"))
  {}
  explicit  CTPPSRPAlignmentInfoReader(int i) {}
  ~CTPPSRPAlignmentInfoReader() override {}
  void analyze(const edm::Event& e, const edm::EventSetup& c) override;
  void printInfo(const CTPPSRPAlignmentCorrectionsData &alignments, const edm::Event& event);
};


void
CTPPSRPAlignmentInfoReader::analyze(const edm::Event& e, const edm::EventSetup& context){
  
  using namespace edm;
  
  
  //this part gets the handle of the event source and the record (i.e. the Database)
  if (e.id().run() == iov_){
    ESHandle<CTPPSRPAlignmentCorrectionsData> alignments;
    context.get<CTPPSRPAlignmentCorrectionsDataRcd>().get(alignments);
    
    // print alignments
    //    printf(" ** RP Alignments\n");
    // for (const auto &p : alignments->ROCMapping)
    //  std::cout << "    " << p.first << " -> " << p.second << std::endl;
    //    time_t unixTime = event.time().unixTime();     
    //char timeStr[50];
    //strftime( timeStr, 50, "%F %T", localtime( &unixTime ) );
    
    std::cout    << "New  alignments found in run="
		 << e.id().run() << ", event=" << e.id().event() 
      //                << ", UNIX timestamp=" << unixTime
      //		 << " (" << timeStr << "):\n"
     		 << "\n" << *alignments <<std::endl;
    
  }
  
  //edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSPixelAnalysisMaskRcd"));
  //if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
  //record not found
  //std::cout <<"Record \"CTPPSPixelAnalysisMaskRcd"<<"\" does not exist "<<std::endl;
  //}
  
  
}

// void CTPPSRPAlignmentInfoReader::printInfo(const CTPPSRPAlignmentCorrectionsData &alignments, const edm::Event& event) const
// {
//   time_t unixTime = event.time().unixTime();
//   char timeStr[50];
//   strftime( timeStr, 50, "%F %T", localtime( &unixTime ) );  
//   edm::LogInfo("CTPPSRPAlignmentInfoReader")
//     << "New  alignments found in run="
//     << event.id().run() << ", event=" << event.id().event() << ", UNIX timestamp=" << unixTime
//     << " (" << timeStr << "):\n"
//     << alignments;
// }

DEFINE_FWK_MODULE(CTPPSRPAlignmentInfoReader);
