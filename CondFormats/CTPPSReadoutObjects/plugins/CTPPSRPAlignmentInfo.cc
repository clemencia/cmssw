/****************************************************************************
 *
 * Authors:
 *  Jan Kaspar
 * Adapted by:
 *  Helena Malbouisson
 *  Clemencia Mora Herrera  
 ****************************************************************************/

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "CondFormats/DataRecord/interface/CTPPSRPAlignmentCorrectionsDataRcd.h"

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsData.h"

//----------------------------------------------------------------------------------------------------

/**
 * \brief Class to print out information on current geometry.
 **/
class CTPPSAlignmentInfo : public edm::one::EDAnalyzer<>
{
public:
  explicit CTPPSAlignmentInfo( const edm::ParameterSet& );

private: 
  std::string alignmentType_;

  edm::ESWatcher<CTPPSRPAlignmentCorrectionsDataRcd> watcherAlignments_;
  //edm::ESWatcher<RPMisalignedAlignmentRecord> watcherMisalignedAlignments_;

  void analyze( const edm::Event&, const edm::EventSetup& ) override;

  void printInfo(const CTPPSRPAlignmentCorrectionsData &alignments, const edm::Event& event) const;
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

CTPPSAlignmentInfo::CTPPSAlignmentInfo( const edm::ParameterSet& iConfig ) :
  alignmentType_   ( iConfig.getUntrackedParameter<std::string>( "alignmentType", "real" ) )
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSAlignmentInfo::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  std::cout << "## CHECK  hello!" <<std::endl;

  edm::ESHandle<CTPPSRPAlignmentCorrectionsData> alignments;
  iSetup.get<CTPPSRPAlignmentCorrectionsDataRcd>().get(alignments);
  std::cout << "## CHECK #0 AlignmentInfo alignments pointer"<< &alignments <<std::endl;


  if ( alignmentType_ == "real" )
    {
      std::cout << "## CHECK #1 AlignmentInfo" <<std::endl;
      if ( watcherAlignments_.check( iSetup ) )
	{
	  iSetup.get<CTPPSRPAlignmentCorrectionsDataRcd>().get( alignments );
	  printInfo(*alignments, iEvent);
	}
      return;
    }

  // else if ( alignmentType_ == "misaligned" )
  //   {
  //     if ( watcherMisalignedAlignments_.check( iSetup ) )
  // 	{
  // 	  iSetup.get<RPMisalignedAlignmentRecord>().get( alignments );
  // 	  printInfo(*alignments, iEvent);
  // 	}
  //     return;
  //   }

  throw cms::Exception("CTPPSAlignmentInfo") << "Unknown geometry type: `" << alignmentType_ << "'.";
}

//----------------------------------------------------------------------------------------------------

void CTPPSAlignmentInfo::printInfo(const CTPPSRPAlignmentCorrectionsData &alignments, const edm::Event& event) const
{
  time_t unixTime = event.time().unixTime();
  char timeStr[50];
  strftime( timeStr, 50, "%F %T", localtime( &unixTime ) );

  edm::LogInfo("CTPPSAlignmentInfo")
    << "New " << alignmentType_ << " alignments found in run="
    << event.id().run() << ", event=" << event.id().event() << ", UNIX timestamp=" << unixTime
    << " (" << timeStr << "):\n"
    << alignments;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE( CTPPSAlignmentInfo );
