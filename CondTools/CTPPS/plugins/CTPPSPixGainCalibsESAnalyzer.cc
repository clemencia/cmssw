#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelGainCalibrations.h"
#include "CondFormats/DataRecord/interface/CTPPSPixelGainCalibrationsRcd.h"
#include "TH2D.h"
#include "TFile.h"

class CTPPSPixGainCalibsESAnalyzer : public edm::EDAnalyzer
{
public:
  explicit  CTPPSPixGainCalibsESAnalyzer(edm::ParameterSet const& p) { 
    std::cout<<"CTPPSPixGainCalibsESAnalyzer"<<std::endl;
  }
  explicit  CTPPSPixGainCalibsESAnalyzer(int i) {
    std::cout<<"CTPPSPixGainCalibsESAnalyzer "<<i<<std::endl; 
  }
  virtual ~CTPPSPixGainCalibsESAnalyzer() {  
    std::cout<<"~CTPPSPixGainCalibsESAnalyzer "<<std::endl;
  }
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c) override;
};

void
CTPPSPixGainCalibsESAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context){
  std::cout<<"###CTPPSPixGainCalibsESAnalyzer::analyze"<<std::endl;
  std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
  std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;
  edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("CTPPSPixelGainCalibrationsRcd"));
  if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
    //record not found
    std::cout <<"Record \"CTPPSPixelGainCalibrationsRcd"<<"\" does not exist "<<std::endl;
  }
  //this part gets the handle of the event source and the record (i.e. the Database)
  edm::ESHandle<CTPPSPixelGainCalibrations> calhandle;
  std::cout<<"got eshandle"<<std::endl;
  context.get<CTPPSPixelGainCalibrationsRcd>().get(calhandle);
  std::cout<<"got context"<<std::endl;
  const CTPPSPixelGainCalibrations* pPixelGainCalibrations=calhandle.product();
  std::cout<<"got CTPPSPixelGainCalibrations* "<< std::endl;
  std::cout<< "print  pointer address : " ;
  std::cout << pPixelGainCalibrations << std::endl;
  
  TFile myfile("output.root","RECREATE");
  myfile.cd();
  
  // the pPixelGainCalibrations object contains the map of detIds  to pixel gains and pedestals for current run
  // we get the map just to loop over the contents, but from here on it should be  as the code (reconstruction etc) needs. 
  // Probably best to check that the key (detid) is in the list before calling the data
  
  std::cout << "Size "  <<  pPixelGainCalibrations->size() << std::endl;
  const CTPPSPixelGainCalibrations::CalibMap & mymap = pPixelGainCalibrations->getCalibMap(); //just to get the keys?
  
  for  (CTPPSPixelGainCalibrations::CalibMap::const_iterator it = mymap.begin(); it !=mymap.end() ; ++it) {
    uint32_t detId = it->first;

    std::cout<<"Address of  detId = "<< (&detId) << " and of it = "<< (&it) << " and of it->first = "<< (&(it->first)) <<std::endl;

    std::cout << "Content  of pPixelGainCalibrations for key: detId= " <<  detId  << std::endl;
    CTPPSPixelGainCalibration  mycalibs0=  pPixelGainCalibrations->getGainCalibration(detId);
    const CTPPSPixelGainCalibration & mycalibs=it->second;

    std::cout<<"Address of  mycalibs0 = "<< (&mycalibs0) << " and of mycalibs = "<< (&mycalibs) << " and of it->second "<<(&(it->second)) << std::endl;


    std::string namep("pedsFromDB_"+ std::to_string(detId));
    std::string nameg("gainsFromDB_"+std::to_string(detId));

    TH2D  mypeds(namep.c_str(),"peds; column; row",156,0.,156.,160,0.,160.);
    TH2D  mygains(nameg.c_str(),"gains; column; row",156,0.,156.,160,0.,160.);
    
    int ncols = mycalibs.getNCols();
    int npix  = mycalibs.getIEnd();
    int nrows = mycalibs.getNRows(); //should be == 160
    std::cout<<"Here ncols = "<<ncols<<" nrows ="<<nrows <<" npix="<<npix <<std::endl;
    for (int jrow=0; jrow < nrows ; ++jrow)
      for(int icol=0; icol < ncols ; ++icol){
	if(mycalibs.isDead(icol+jrow*ncols))
	  {std::cout<<"Dead Pixel icol ="<< icol << " jrow ="<< jrow <<std::endl; continue;}
	if(mycalibs.isNoisy(icol+jrow*ncols))
	  {std::cout<<"Noisy Pixel icol ="<< icol << " jrow ="<< jrow <<std::endl;continue;}
	mygains.Fill(icol,jrow,mycalibs.getGain(icol,jrow));
	mypeds.Fill(icol,jrow,mycalibs.getPed(icol,jrow));	
      }
    
    mypeds.Write();
    mygains.Write();
  }
  myfile.Write();
  myfile.Close();
}
DEFINE_FWK_MODULE(CTPPSPixGainCalibsESAnalyzer);

