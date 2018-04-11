/****************************************************************************
 *
 * This is a part of CMS-TOTEM PPS offline software.
 * Authors: 
 *  Jan Kaspar (jan.kaspar@gmail.com) 
 *  Helena Malbouisson 
 *  Clemencia Mora Herrera
 ****************************************************************************/

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"


#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsMethods.h"
//#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSRPAlignmentCorrectionsData.h"
#include "CondFormats/DataRecord/interface/CTPPSRPAlignmentCorrectionsDataRcd.h"

// #include "CondFormats/AlignmentRecord/interface/RPMeasuredAlignmentRecord.h"
// #include "CondFormats/AlignmentRecord/interface/RPRealAlignmentRecord.h"
// #include "CondFormats/AlignmentRecord/interface/RPMisalignedAlignmentRecord.h"

#include <vector>
#include <string>
#include <map>
#include <set>


/**
 * Loads alignment corrections to EventSetup.
 **/
class  CTPPSRPAlignmentCorrectionsDataESSourceXML : public edm::ESProducer , public edm::EventSetupRecordIntervalFinder
{
public:
  CTPPSRPAlignmentCorrectionsDataESSourceXML(const edm::ParameterSet &p);
  ~CTPPSRPAlignmentCorrectionsDataESSourceXML() override;

  std::unique_ptr<CTPPSRPAlignmentCorrectionsData> produceMeasured(const CTPPSRPAlignmentCorrectionsDataRcd &);
  // std::unique_ptr<CTPPSRPAlignmentCorrectionsData> produceReal(const RPRealAlignmentRecord &);
  // std::unique_ptr<CTPPSRPAlignmentCorrectionsData> produceMisaligned(const RPMisalignedAlignmentRecord &);

protected:
  unsigned int verbosity;
  //  RPAlignmentCorrectionsDataSequence acsMeasured, acsReal, acsMisaligned;
  CTPPSRPAlignmentCorrectionsData acMeasured;

  void setIntervalFor(const edm::eventsetup::EventSetupRecordKey&, const edm::IOVSyncValue&, edm::ValidityInterval&) override;

  // static edm::EventID previousLS(const edm::EventID &src)
  // {
  //   if (src.run() == edm::EventID::maxRunNumber() && src.luminosityBlock() == edm::EventID::maxLuminosityBlockNumber())
  //     return src;

  //   if (src.luminosityBlock() == 0)
  //     return edm::EventID(src.run() - 1, edm::EventID::maxLuminosityBlockNumber(), src.event());

  //   return edm::EventID(src.run(), src.luminosityBlock() - 1, src.event());
  // }

  // static edm::EventID nextLS(const edm::EventID &src)
  // {
  //   if (src.luminosityBlock() == edm::EventID::maxLuminosityBlockNumber())
  //     {
  //       if (src.run() == edm::EventID::maxRunNumber())
  //         return src;

  //       return edm::EventID(src.run() + 1, 0, src.event());
  //     }

  //   return edm::EventID(src.run(), src.luminosityBlock() + 1, src.event());
  // }

  // /// merges an array of sequences to one
  // RPAlignmentCorrectionsDataSequence Merge(const std::vector<RPAlignmentCorrectionsDataSequence>&) const;

  /// builds a sequence of corrections from provided sources and runs a few checks
  //  void PrepareSequence(const std::string &label, RPAlignmentCorrectionsDataSequence &seq, const std::vector<std::string> &files) const;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

CTPPSRPAlignmentCorrectionsDataESSourceXML::CTPPSRPAlignmentCorrectionsDataESSourceXML(const edm::ParameterSet &pSet) :
  verbosity(pSet.getUntrackedParameter<unsigned int>("verbosity", 0))
{
  std::string measuredFiles;
  const auto &f= pSet.getParameter<std::string>("XMLFile");
  measuredFiles=edm::FileInPath(f).fullPath();
  //  PrepareSequence("Measured", acsMeasured, measuredFiles);
  acMeasured =  CTPPSRPAlignmentCorrectionsMethods::loadFromXML(measuredFiles); // only 1 file !
  std::cout << "## CHECK #1 acmeasured pointer "<< &acMeasured <<std::endl;
  unsigned int rpid = 2054160384 ;
  CTPPSRPAlignmentCorrectionData acrp = acMeasured.getRPCorrection(rpid);
  std::cout << "## CHECK #1.5 acrp for example rp 2054160384  "<< acrp.getShX() <<std::endl;

  // std::vector<std::string> realFiles;
  // for (const auto &f: pSet.getParameter< vector<string> >("RealFiles"))
  //   realFiles.push_back(edm::FileInPath(f).fullPath());
  // PrepareSequence("Real", acsReal, realFiles);

  // std::vector<std::string> misalignedFiles;
  // for (const auto &f: pSet.getParameter< vector<string> >("MisalignedFiles"))
  //   misalignedFiles.push_back(edm::FileInPath(f).fullPath());
  // PrepareSequence("Misaligned", acsMisaligned, misalignedFiles);

  setWhatProduced(this, &CTPPSRPAlignmentCorrectionsDataESSourceXML::produceMeasured);

  std::cout << "## CHECK #2 after setwhaproduced " << &acMeasured << std::endl;

  // setWhatProduced(this, &CTPPSRPAlignmentCorrectionsDataESSourceXML::produceReal);
  // setWhatProduced(this, &CTPPSRPAlignmentCorrectionsDataESSourceXML::produceMisaligned);

  findingRecord<CTPPSRPAlignmentCorrectionsDataRcd>();
  std::cout << "## CHECK #3 " <<std::endl;

  // findingRecord<RPRealAlignmentRecord>();
  // findingRecord<RPMisalignedAlignmentRecord>();
}

//----------------------------------------------------------------------------------------------------


CTPPSRPAlignmentCorrectionsDataESSourceXML::~CTPPSRPAlignmentCorrectionsDataESSourceXML()
{
}

//----------------------------------------------------------------------------------------------------

// RPAlignmentCorrectionsDataSequence CTPPSRPAlignmentCorrectionsDataESSourceXML::Merge(const vector<RPAlignmentCorrectionsDataSequence>& seqs) const
// {
//   // find interval boundaries
//   map< edm::EventID, vector< pair<bool, const CTPPSRPAlignmentCorrectionsData*> > > bounds;

//   for (const auto & seq : seqs)
//     {
//       for (const auto &p : seq)
// 	{
// 	  const ValidityInterval &iov = p.first;
// 	  const CTPPSRPAlignmentCorrectionsData *corr = & p.second;

// 	  const EventID &event_first = iov.first().eventID();
// 	  bounds[event_first].emplace_back( pair<bool, const CTPPSRPAlignmentCorrectionsData*>(true, corr) );

// 	  const EventID &event_after = nextLS(iov.last().eventID());
// 	  bounds[event_after].emplace_back( pair<bool, const CTPPSRPAlignmentCorrectionsData*>(false, corr) );
// 	}
//     }

//   // build correction sums per interval
//   set<const CTPPSRPAlignmentCorrectionsData*> accumulator;
//   RPAlignmentCorrectionsDataSequence result;
//   for (map< EventID, vector< pair<bool, const CTPPSRPAlignmentCorrectionsData*> > >::const_iterator tit = bounds.begin(); tit != bounds.end(); ++tit)
//     {
//       for (const auto & cit : tit->second)
// 	{
// 	  bool add = cit.first;
// 	  const CTPPSRPAlignmentCorrectionsData *corr = cit.second;

// 	  if (add)
// 	    accumulator.insert(corr);
// 	  else 
// 	    accumulator.erase(corr);
// 	}

//       auto tit_next = tit;
//       tit_next++;
//       if (tit_next == bounds.end())
// 	break;

//       const EventID &event_first = tit->first;
//       const EventID &event_last = previousLS(tit_next->first);

//       if (verbosity)
// 	{
// 	  LogVerbatim("CTPPSRPAlignmentCorrectionsDataESSourceXML")
// 	    << "    first=" << RPAlignmentCorrectionsMethods::iovValueToString(edm::IOVSyncValue(event_first))
// 	    << ", last=" << RPAlignmentCorrectionsMethods::iovValueToString(edm::IOVSyncValue(event_last))
// 	    << ": alignment blocks " << accumulator.size();
// 	}

//       CTPPSRPAlignmentCorrectionsData corr_sum;
//       for (auto sit : accumulator)
// 	corr_sum.addCorrections(*sit);

//       result.insert(edm::ValidityInterval(edm::IOVSyncValue(event_first), edm::IOVSyncValue(event_last)), corr_sum);
//     }

//   return result;
// }

//----------------------------------------------------------------------------------------------------

// void CTPPSRPAlignmentCorrectionsDataESSourceXML::PrepareSequence(const string &label, RPAlignmentCorrectionsDataSequence &seq, const vector<string> &files) const
// {
//   if (verbosity)
//     LogVerbatim(">> CTPPSRPAlignmentCorrectionsDataESSourceXML") << "CTPPSRPAlignmentCorrectionsDataESSourceXML::PrepareSequence(" << label << ")";
//   vector<RPAlignmentCorrectionsDataSequence> sequences;
//   for (const auto & file : files)
//     sequences.emplace_back(RPAlignmentCorrectionsMethods::loadFromXML(file));
//   seq = Merge(sequences);
// }
//----------------------------------------------------------------------------------------------------

std::unique_ptr<CTPPSRPAlignmentCorrectionsData> CTPPSRPAlignmentCorrectionsDataESSourceXML::produceMeasured(const CTPPSRPAlignmentCorrectionsDataRcd &iRecord)
{
  return std::make_unique<CTPPSRPAlignmentCorrectionsData>(acMeasured);
}



//----------------------------------------------------------------------------------------------------

void CTPPSRPAlignmentCorrectionsDataESSourceXML::setIntervalFor(const edm::eventsetup::EventSetupRecordKey &key,
						   const IOVSyncValue& iosv, ValidityInterval& valInt) 
{
  if (verbosity)
    {
      time_t unixTime = iosv.time().unixTime();
      char timeStr[50];
      strftime(timeStr, 50, "%F %T", localtime(&unixTime));

      LogVerbatim("CTPPSIncludeAlignmentsFromXML")
	<< ">> CTPPSIncludeAlignmentsFromXML::setIntervalFor(" << key.name() << ")";

      LogVerbatim("CTPPSIncludeAlignmentsFromXML")
	<< "    event=" << iosv.eventID() << ", UNIX timestamp=" << unixTime << " (" << timeStr << ")";
    }

  // // determine what sequence and corrections should be used
  // // RPAlignmentCorrectionsDataSequence *p_seq = nullptr;
  CTPPSRPAlignmentCorrectionsData *p_corr = nullptr;
  
  if (strcmp(key.name(), "CTPPSRPAlignmentCorrectionsDataRcd") == 0)
    {
      //     //p_seq = &acsMeasured;
      p_corr = &acMeasured;
    }

  // // if (strcmp(key.name(), "RPRealAlignmentRecord") == 0)
  // //   {
  // //     p_seq = &acsReal;
  // //     p_corr = &acReal;
  // //   }

  // // if (strcmp(key.name(), "RPMisalignedAlignmentRecord") == 0)
  // //   {
  // //     p_seq = &acsMisaligned;
  // //     p_corr = &acMisaligned;
  // //   }

   if (p_corr == nullptr)
     throw cms::Exception("CTPPSIncludeAlignmentsFromXML::setIntervalFor") << "Unknown record " << key.name();
   std::cout << "CHECK p_corr =>" << p_corr<<std::endl;

   // // find the corresponding interval
   bool next_exists = false;
   const edm::EventID &event_curr = iosv.eventID();
   edm::EventID event_next_start(edm::EventID::maxRunNumber(), edm::EventID::maxLuminosityBlockNumber(), 1);

  // // for (const auto &it: *p_seq)
  // //   {
  // //     const auto &it_event_first = it.first.first().eventID();
  // //     const auto &it_event_last = it.first.last().eventID();

  // //     bool it_contained_lo = ( (it_event_first.run() < event_curr.run()) ||
  // // 			       ((it_event_first.run() == event_curr.run()) && (it_event_first.luminosityBlock() <= event_curr.luminosityBlock())) );

  // //     bool it_contained_up = ( (it_event_last.run() > event_curr.run()) ||
  // // 			       ((it_event_last.run() == event_curr.run()) && (it_event_last.luminosityBlock() >= event_curr.luminosityBlock())) );

  // //     if (it_contained_lo && it_contained_up)
  // // 	{
  // // 	  valInt = it.first;
  // // 	  *p_corr = it.second;

  // // 	  if (verbosity)
  // // 	    {
  // // 	      LogVerbatim("CTPPSIncludeAlignmentsFromXML")
  // // 		<< "    setting validity interval ["
  // // 		<< RPAlignmentCorrectionsMethods::iovValueToString(valInt.first())
  // // 		<< ", " << RPAlignmentCorrectionsMethods::iovValueToString(valInt.last()) << "]";
  // // 	    }

  // // 	  return;
  // // 	}

  // //     bool it_in_future = ( (it_event_first.run() > event_curr.run()) ||
  // // 			    ((it_event_first.run() == event_curr.run() && (it_event_first.luminosityBlock() > event_curr.luminosityBlock()))) );

  // //     if (it_in_future)
  // // 	{
  // // 	  next_exists = true;
  // // 	  if (event_next_start > it_event_first)
  // // 	    event_next_start = it_event_first;
  // // 	}
  // //   }

  // // no interval found, set empty corrections
  // *p_corr = CTPPSRPAlignmentCorrectionsData();

  // if (!next_exists)
  //   {
   valInt = ValidityInterval(iosv, iosv.endOfTime());
  //   } else {
  //   //const EventID &event_last = previousLS(event_next_start);
  //   //valInt = ValidityInterval(iosv, IOVSyncValue(event_last));
  // }

  // if (verbosity)
  //   {
  //     LogVerbatim("CTPPSIncludeAlignmentsFromXML")
  // 	<< "    setting validity interval ["
  // 	<< CTPPSRPAlignmentCorrectionsMethods::iovValueToString(valInt.first())
  // 	<< ", " << CTPPSRPAlignmentCorrectionsMethods::iovValueToString(valInt.last()) << "] (empty alignment corrections)";
  //   }
}

DEFINE_FWK_EVENTSETUP_SOURCE(CTPPSRPAlignmentCorrectionsDataESSourceXML);
