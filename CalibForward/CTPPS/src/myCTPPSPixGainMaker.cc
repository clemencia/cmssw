1;2c// -*- C++ -*-
//
// Package:    CalibForward/CTPPS
// Class:      myCTPPSPixGainMaker
// 
/**\class myCTPPSPixGainMaker myCTPPSPixGainMaker.cc CalibForward/CTPPS/plugins/myCTPPSPixGainMaker.cc

 Description: analyzer to create SQlite for CTPPS Pixel Gain Calibration

 Implementation:
     Adapted from SiPixelGainCalibrationReadDQMFile 
*/
//
// Original Author:  Clemencia Mora Herrera
//         Created:  Fri, 11 Nov 2016 18:20:06 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
//CTPPS Gain Calibration Conditions Object 
#include "CondFormats/CTPPSObjects/interface/CTPPSPixelGainCalibration.h"
//CTPPS tracker DetId
#include "DataFormats/CTPPSDetId/interface/CTPPSTrackerDetId.h"
#include "TFile.h"

//
// class declaration
//


class myCTPPSPixGainMaker : public edm::one::EDAnalyzer<>  
{
 public:
  explicit myCTPPSPixGainMaker(const edm::ParameterSet&);
  ~myCTPPSPixGainMaker();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
 private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  void getHistos();
  void fillDB();
  // ----------member data ---------------------------
  std::string m_record;
  std::string m_inputHistosFileName;
  TFile * m_inputRootFile;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
myCTPPSPixGainMaker::myCTPPSPixGainMaker(const edm::ParameterSet& iConfig):   
  m_record(iConfig.getUntrackedParameter<std::string>("record","CTPPSPixelGainCalibrationRcd")),
  m_inputHistosFileName(iConfig.getUntrackedParameter<std::string>("inputrootfile","inputfile.root"))
{

}


myCTPPSPixGainMaker::~myCTPPSPixGainMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
myCTPPSPixGainMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //   using namespace edm;

}


// ------------ method called once each job just before starting event loop  ------------
void 
myCTPPSPixGainMaker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
myCTPPSPixGainMaker::endJob() 
{
  getHistos();
  fillDB();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
myCTPPSPixGainMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void 
myCTPPSPixGainMaker::getHistos() 
{
  std::cout <<"Parsing file " <<m_inputHistosFileName << std::endl;
  m_inputRootFile = new TFile(m_inputHistosFileName.c_str());
  m_inputRootFile->cd();
  TDirectory *dir = m_inputRootFile_->GetDirectory("siPixelGainCalibrationAnalysis");
  TList *list = dir->GetListOfKeys();

}


void 
myCTPPSPixGainMaker::fillDB() 
{
  
  
  //hardcoded values for test only
  CTPPSPixelGainCalibration theGainCalibrationDbInput1(0.0,1.0,0.0,1.0);
  std::vector<char> theCTPPSPixelGainCalibPerPixel;
  std::cout<<"size of vector: "<<  theCTPPSPixelGainCalibPerPixel.size()<<std::endl;

  // testing the class
  theGainCalibrationDbInput1.setData(0.5,0.6,theCTPPSPixelGainCalibPerPixel);
  int   vecsize=theCTPPSPixelGainCalibPerPixel.size();
  std::cout << "size of vector: "<< vecsize<<std::endl;
  int i=0;
  while (i<vecsize){
      std::cout << "vector["<< i << "]"<< theCTPPSPixelGainCalibPerPixel[i]<<std::endl;
      i++;
  }
  CTPPSPixelGainCalibration::Range range(theCTPPSPixelGainCalibPerPixel.begin(),theCTPPSPixelGainCalibPerPixel.end());
  CTPPSTrackerDetId myid020(/*arm*/0,/*station*/0,/*pot*/2,/*plane*/0);
  //  CTPPSTrackerDetId myid135(    1,           0,       3,         5);
  int ncols=52;
  //int nrows=80;
  if(!theGainCalibrationDbInput1.put(myid020.rawId(),range,ncols))
    edm::LogError("CTPPSPixelGainCalibrationAnalysis")<<"warning: detid already exists for Offline calibration database"<<std::endl;

  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if(!mydbservice.isAvailable() ){
    edm::LogError("db service unavailable");
    return;
  }
  mydbservice->writeOne(&theGainCalibrationDbInput1, mydbservice->currentTime(), m_record  );
  
  
}




//define this as a plug-in
DEFINE_FWK_MODULE(myCTPPSPixGainMaker);
