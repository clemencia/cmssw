// -*- C++ -*-
//
// Package:    CalibForward/CTPPS
// Class:      myCTPPSRPPosMaker
// 
/**\class myCTPPSRPPosMaker myCTPPSRPPosMaker.cc CalibForward/CTPPS/plugins/myCTPPSRPPosMaker.cc

 Description: class to make RP position conditions

*/
//
// Original Author:  Clemencia Mora Herrera
//         Created:  Mon, 27 Jun 2016 21:04:36 GMT
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
#include "CondFormats/CTPPSObjects/interface/CTPPSRPPositions.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class myCTPPSRPPosMaker : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit myCTPPSRPPosMaker(const edm::ParameterSet&);
      ~myCTPPSRPPosMaker();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      std::string m_record;

      // ----------member data ---------------------------
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
myCTPPSRPPosMaker::myCTPPSRPPosMaker(const edm::ParameterSet& iConfig):
  m_record(iConfig.getParameter<std::string>("record"))
{
   //now do what ever initialization is needed
   usesResource("TFileService");

}


myCTPPSRPPosMaker::~myCTPPSRPPosMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
myCTPPSRPPosMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


CTPPSRPPositions* pRPPositions = new CTPPSRPPositions();
 pRPPositions->setRPPosition(0,41.287);
// Form the data here

edm::Service<cond::service::PoolDBOutputService> poolDbService;
if( poolDbService.isAvailable() )
  poolDbService->writeOne( pRPPositions, poolDbService->currentTime(),
			   m_record  );
///should I use PopCon instead (template service)


// #ifdef THIS_IS_AN_EVENT_EXAMPLE
//    Handle<ExampleData> pIn;
//    iEvent.getByLabel("example",pIn);
// #endif
// #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//    ESHandle<SetupData> pSetup;
//    iSetup.get<SetupRecord>().get(pSetup);
// #endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
myCTPPSRPPosMaker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
myCTPPSRPPosMaker::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
myCTPPSRPPosMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(myCTPPSRPPosMaker);
