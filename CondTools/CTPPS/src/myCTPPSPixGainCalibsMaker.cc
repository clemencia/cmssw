#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
//CTPPS Gain Calibration Conditions Object 
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelGainCalibrations.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelGainCalibration.h"
#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelIndices.h"
//CTPPS tracker DetId
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "TFile.h"
#include "TH2F.h"
#include <iostream>
#include <vector>
#include <string>

//
// class declaration
//


class myCTPPSPixGainCalibsMaker : public edm::one::EDAnalyzer<>  
{
 public:
  explicit myCTPPSPixGainCalibsMaker(const edm::ParameterSet&);
  ~myCTPPSPixGainCalibsMaker();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
 private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  void getHistos();
  void fillDB();
  void getGainsPedsFromHistos(uint32_t detid,int rocId, int index, std::vector<double>&peds,std::vector<double>&gains,std::map<int,int> & myindxmap, int nrocs);
  // ----------Member data ---------------------------
  std::string m_record;
  std::string m_inputHistosFileName;
  TFile * m_inputRootFile;
  std::map<uint32_t, std::vector<std::string> > detidSlopeNameMap;
  std::map<uint32_t, std::vector<std::string> > detidInterceptNameMap;
  std::map<uint32_t, std::vector<int>  > detidROCsPresent;
};


//
// constructors and destructor
//
myCTPPSPixGainCalibsMaker::myCTPPSPixGainCalibsMaker(const edm::ParameterSet& iConfig):   
  m_record(iConfig.getUntrackedParameter<std::string>("record","CTPPSPixelGainCalibrationsRcd")),
  m_inputHistosFileName(iConfig.getUntrackedParameter<std::string>("inputrootfile","inputfile.root"))
{

}


myCTPPSPixGainCalibsMaker::~myCTPPSPixGainCalibsMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void
myCTPPSPixGainCalibsMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //   using namespace edm;

}


// ------------ method called once each job just before starting event loop  ------------
void 
myCTPPSPixGainCalibsMaker::beginJob()
{
}


// ------------ method called once each job just after ending the event loop  ------------
void 
myCTPPSPixGainCalibsMaker::endJob() 
{
  getHistos();
  fillDB();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
myCTPPSPixGainCalibsMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void 
myCTPPSPixGainCalibsMaker::getHistos() 
{
  std::cout <<"Parsing file " <<m_inputHistosFileName << std::endl;
  m_inputRootFile = new TFile(m_inputHistosFileName.c_str());
  m_inputRootFile->cd();
  //  TDirectory *dir = m_inputRootFile->GetDirectory("CTPPS");
  //  TList *list = dir->GetListOfKeys();
  //  int ndir= list->GetEntries();
  //  std::cout<<"ndir= " <<ndir<<std::endl;


  int sector[2] = {45,56};
  int station[1]={2};
  int pot[1]={3};
  

  for(int arm=0 ; arm<2 ; arm++)
    for(int plane=0 ; plane<6 ; plane++){
      CTPPSPixelDetId mytempid(arm,station[0],pot[0],plane);
      std::vector<std::string> slopenamevec;
      std::vector<std::string> intrcptnamevec;
      std::vector<int> listrocs;
      for(int roc=0 ; roc<6 ; roc++){
	char temppathslope[200];
	char temppathintercept[200];
	int i = arm;
	sprintf(temppathslope,"CTPPS/CTPPS_SEC%d/CTPPS_SEC%d_RP%d%d%d/CTPPS_SEC%d_RP%d%d%d_PLN%d/CTPPS_SEC%d_RP%d%d%d_PLN%d_ROC%d_Slope2D",sector[i],sector[i],arm,station[0],pot[0],sector[i],arm,station[0],pot[0],plane,sector[i],arm,station[0],pot[0],plane,roc);

	sprintf(temppathintercept,"CTPPS/CTPPS_SEC%d/CTPPS_SEC%d_RP%d%d%d/CTPPS_SEC%d_RP%d%d%d_PLN%d/CTPPS_SEC%d_RP%d%d%d_PLN%d_ROC%d_Intercept2D",sector[i],sector[i],arm,station[0],pot[0],sector[i],arm,station[0],pot[0],plane,sector[i],arm,station[0],pot[0],plane,roc);
	
	std::string pathslope(temppathslope);
	std::string pathintercept(temppathintercept);
	if( m_inputRootFile->Get(pathslope.c_str()) &&  m_inputRootFile->Get(pathintercept.c_str())){
	  slopenamevec.push_back(pathslope);
	  intrcptnamevec.push_back(pathintercept);
	  listrocs.push_back(roc);
	}
      }
      
      detidSlopeNameMap[mytempid.rawId()]     = slopenamevec;
      detidInterceptNameMap[mytempid.rawId()] = intrcptnamevec;
      detidROCsPresent[mytempid.rawId()]      = listrocs;
    }
}

  


void 
myCTPPSPixGainCalibsMaker::fillDB() 
{  
  //hardcoded values for test only
  CTPPSPixelGainCalibrations * gainCalibsTest = new CTPPSPixelGainCalibrations();
  CTPPSPixelGainCalibrations * gainCalibsTest1= new CTPPSPixelGainCalibrations();
  //  CTPPSPixelDetId myid020(/*arm*/0,/*station*/0,/*pot*/2,/*plane*/0);

  std::cout<<"Here! "<<std::endl;  
 
  for (std::map<uint32_t,std::vector<int> >::iterator it=detidROCsPresent.begin(); it!=detidROCsPresent.end(); ++it){
         uint32_t tempdetid= it->first;
	 //	 std::vector<float> realgains,realpeds;
	 std::vector<int> rocs = it->second;
	 unsigned int nrocs = rocs.size();
	 std::map<int,int> mapIPixIndx ;
	 
	 std::vector<double> gainsFromHistos;
	 std::vector<double> pedsFromHistos;

	 CTPPSPixelGainCalibration tempPGCalib;

	 //	 std::cout<<"Here 2! detid= "<< tempdetid.rawId()<<std::endl;  

	 for (unsigned int i = 0; i<nrocs ; i++){
	   //	   std::cout << "Here 2.5! detidSlopeNameMap[detid][i]" << detidSlopeNameMap[tempdetid][i] << std::endl;
   	   getGainsPedsFromHistos(tempdetid,i, rocs[i], pedsFromHistos,gainsFromHistos,mapIPixIndx,nrocs);

	 }

	 std::cout<<"Here testing!"<<std::endl;
	 if(gainsFromHistos.size()!=nrocs*52*80)
	   std::cout<<"Something is wrong"<< gainsFromHistos.size()<<std::endl;

	 std::vector<float> orderedGains;
	 std::vector<float> orderedPeds;
	 for (unsigned int k = 0 ; k < nrocs*52*80 ; k++){
	   int indx      = mapIPixIndx[k];
	   float tmpped  = pedsFromHistos[indx];
	   float tmpgain = gainsFromHistos[indx]; 
	   orderedGains.push_back(tmpgain);
           orderedPeds.push_back(tmpped);
	   tempPGCalib.putData(k,tmpped,tmpgain);
	 }


	 std::cout << "N ROCs = "<<  nrocs<< " N pixels = "<< orderedPeds.size()<< std::endl;
	 gainCalibsTest->setGainCalibration(tempdetid,orderedPeds,orderedGains);
	 gainCalibsTest1->setGainCalibration(tempdetid,tempPGCalib);
  }
  std::cout<<" Here 3!"<<std::endl;
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if(!mydbservice.isAvailable() ){
    edm::LogError("db service unavailable");
    return;
  }
  mydbservice->writeOne( gainCalibsTest, mydbservice->currentTime(), m_record  );

}

void 
myCTPPSPixGainCalibsMaker::getGainsPedsFromHistos(uint32_t detid, int ROCindex, int rocId, std::vector<double> &peds,std::vector<double> &gains, std::map<int,int> & mymap, int nrocs) {
  CTPPSPixelIndices  modulepixels(52*nrocs/2,160);
  TH2D * tempslope   = (TH2D*) m_inputRootFile->Get(detidSlopeNameMap[detid][ROCindex].c_str()); 
  TH2D * tempintrcpt = (TH2D*) m_inputRootFile->Get(detidInterceptNameMap[detid][ROCindex].c_str()); 
  int ncols = tempslope->GetNbinsX(); 
  int nrows = tempslope->GetNbinsY();
  if (nrows != 80  || ncols != 52 )
    std::cout<<"Something wrong ncols = "<< ncols << " and nrows = " << nrows <<std::endl;
  
  for (int jrow = 0; jrow < nrows ; ++jrow) // when scanning through the 2d histo make sure to avoid underflow bin i or j ==0
    for (int icol = 0 ; icol < ncols ; ++icol){
      double tmpslp  = tempslope->GetBinContent(icol+1,jrow+1); 
      double tmpgain = (tmpslp == 0.0) ? 0.0 : 1.0/tmpslp;
      double tmpped  = tempintrcpt->GetBinContent(icol+1,jrow+1);
      gains.push_back(tmpgain);
      peds.push_back(tmpped);
      int modCol=-1;
      int modRow=-1;
      modulepixels.transformToModule(icol,jrow, rocId ,modCol,modRow);
      int indx     = gains.size()-1;
      int pixIndx  = modCol + modRow * (52*nrocs/2);
      mymap[pixIndx]=indx; 
    }
}


//define this as a plug-in
DEFINE_FWK_MODULE(myCTPPSPixGainCalibsMaker);
