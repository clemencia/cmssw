#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondFormats/CTPPSObjects/interface/CTPPSRPPositions.h"
#include "CondFormats/DataRecord/interface/CTPPSRPPositionsRcd.h"

namespace edmtest
{
  class myCTPPSRPPosESAnalyzer : public edm::EDAnalyzer
  {
  public:
    explicit  myCTPPSRPPosESAnalyzer(edm::ParameterSet const& p) { 
      std::cout<<"myCTPPSRPPosESAnalyzer"<<std::endl;
          }
    explicit  myCTPPSRPPosESAnalyzer(int i) {
      std::cout<<"myCTPPSRPPosESAnalyzer "<<i<<std::endl; 
    }
    virtual ~myCTPPSRPPosESAnalyzer() {  
      std::cout<<"~myCTPPSRPPosESAnalyzer "<<std::endl;
    }
        virtual void analyze(const edm::Event& e, const edm::EventSetup& c) override;
  };
  
  void
  myCTPPSRPPosESAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context){
    std::cout<<"###myCTPPSRPPosESAnalyzer::analyze"<<std::endl;
    std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
    std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;
    edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSRPPositionsRcd"));
    if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
      //record not found
      std::cout <<"Record \"CTPPSRPPositionsRcd"<<"\" does not exist "<<std::endl;
    }
    edm::ESHandle<CTPPSRPPositions> poshandle;
    std::cout<<"got eshandle"<<std::endl;
    context.get<CTPPSRPPositionsRcd>().get(poshandle);
    std::cout<<"got context"<<std::endl;
    const CTPPSRPPositions* pRPPositions=poshandle.product();
    std::cout<<"got CTPPSRPPositions* "<< std::endl;
    std::cout<< "print  pointer address : " ;
    std::cout << pRPPositions << std::endl;
    
    std::cout << "Size "  <<  pRPPositions->size() << std::endl;

   const CTPPSRPPositions::posmap & mymap = pRPPositions->getPosmap();
   
   for  (CTPPSRPPositions::posmap::const_iterator it = mymap.begin(); it !=mymap.end() ; ++it) 
     std::cout<<"Content  of pRPPositions for key: "<< it->first <<std::endl 
	      <<" values dist: "<< pRPPositions->getRPDistBPCenter(it->first)<<std::endl  
	      <<" offset " << (it->second).getOffset()<<std::endl
	      <<" motor " << (it->second).getRawMotor()<<std::endl
	      <<" lvd    " << (it->second).getRawLVD()<<std::endl
	      <<" resolver " << (it->second).getRawResolver()<<std::endl
	      <<std::endl;
   
  }
  DEFINE_FWK_MODULE(myCTPPSRPPosESAnalyzer);
}
