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


class WriteCTPPSPixGainCalibrations : public edm::one::EDAnalyzer<>  
{
 public:
  explicit WriteCTPPSPixGainCalibrations(const edm::ParameterSet&);
  ~WriteCTPPSPixGainCalibrations();
  
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
  std::map<uint32_t, std::vector<std::string> > detidHistoNameMap;
  //  std::map<uint32_t, std::vector<std::string> > detidSlopeNameMap;
  //  std::map<uint32_t, std::vector<std::string> > detidInterceptNameMap;
  //  std::map<uint32_t, std::vector<std::string> > detidChi2NameMap;
  //  std::map<uint32_t, std::vector<std::string> > detidNpfitNameMap;
  std::map<uint32_t, std::vector<int>  > detidROCsPresent;
};


//
// constructors and destructor
//
WriteCTPPSPixGainCalibrations::WriteCTPPSPixGainCalibrations(const edm::ParameterSet& iConfig):   
  m_record(iConfig.getUntrackedParameter<std::string>("record","CTPPSPixelGainCalibrationsRcd")),
  m_inputHistosFileName(iConfig.getUntrackedParameter<std::string>("inputrootfile","inputfile.root"))
{

}


WriteCTPPSPixGainCalibrations::~WriteCTPPSPixGainCalibrations()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void
WriteCTPPSPixGainCalibrations::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //   using namespace edm;

}


// ------------ method called once each job just before starting event loop  ------------
void 
WriteCTPPSPixGainCalibrations::beginJob()
{
}


// ------------ method called once each job just after ending the event loop  ------------
void 
WriteCTPPSPixGainCalibrations::endJob() 
{
  getHistos();
  fillDB();
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
WriteCTPPSPixGainCalibrations::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


void 
WriteCTPPSPixGainCalibrations::getHistos() 
{
  std::cout <<"Parsing file " <<m_inputHistosFileName << std::endl;
  m_inputRootFile = new TFile(m_inputHistosFileName.c_str());
  m_inputRootFile->cd();


  int sector[2] = {45,56};
  int station[1]={2};
  int pot[1]={3};
  

  for(int arm=0 ; arm<2 ; arm++)
    for(int plane=0 ; plane<6 ; plane++){
      CTPPSPixelDetId mytempid(arm,station[0],pot[0],plane);
      //      std::vector<std::string> slopenamevec;
      //      std::vector<std::string> intrcptnamevec;
      std::vector<std::string> histnamevec;
      std::vector<int> listrocs;
      for(int roc=0 ; roc<6 ; roc++){
	char temppathhistos[200];
	//	char temppathintercept[200];
	int i = arm;
	//	sprintf(temppathslope,"CTPPS/CTPPS_SEC%d/CTPPS_SEC%d_RP%d%d%d/CTPPS_SEC%d_RP%d%d%d_PLN%d/CTPPS_SEC%d_RP%d%d%d_PLN%d_ROC%d_Slope2D",sector[i],sector[i],arm,station[0],pot[0],sector[i],arm,station[0],pot[0],plane,sector[i],arm,station[0],pot[0],plane,roc);

	//	sprintf(temppathintercept,"CTPPS/CTPPS_SEC%d/CTPPS_SEC%d_RP%d%d%d/CTPPS_SEC%d_RP%d%d%d_PLN%d/CTPPS_SEC%d_RP%d%d%d_PLN%d_ROC%d_Intercept2D",sector[i],sector[i],arm,station[0],pot[0],sector[i],arm,station[0],pot[0],plane,sector[i],arm,station[0],pot[0],plane,roc);

	sprintf(temppathhistos,"CTPPS/CTPPS_SEC%d/CTPPS_SEC%d_RP%d%d%d/CTPPS_SEC%d_RP%d%d%d_PLN%d/CTPPS_SEC%d_RP%d%d%d_PLN%d_ROC%d",sector[i],sector[i],arm,station[0],pot[0],sector[i],arm,station[0],pot[0],plane,sector[i],arm,station[0],pot[0],plane,roc);

        std::string pathhistos(temppathhistos);
	std::string pathslope    = pathhistos + "_Slope2D";
       	std::string pathintercept= pathhistos + "_Intercept2D";
	if( m_inputRootFile->Get(pathslope.c_str()) &&  m_inputRootFile->Get(pathintercept.c_str())){
	  //	  slopenamevec.push_back(pathslope);
	  //	  intrcptnamevec.push_back(pathintercept);
	  histnamevec.push_back(pathhistos);
	  listrocs.push_back(roc);
	}
      }
      detidHistoNameMap[mytempid.rawId()]      = histnamevec;
      //      detidSlopeNameMap[mytempid.rawId()]     = slopenamevec;
      //      detidInterceptNameMap[mytempid.rawId()] = intrcptnamevec;
      detidROCsPresent[mytempid.rawId()]      = listrocs;
    }
}

  


void 
WriteCTPPSPixGainCalibrations::fillDB() 
{  
  CTPPSPixelGainCalibrations * gainCalibsTest = new CTPPSPixelGainCalibrations();
  CTPPSPixelGainCalibrations * gainCalibsTest1= new CTPPSPixelGainCalibrations();

  std::cout<<"Here! "<<std::endl;  
 
  for (std::map<uint32_t,std::vector<int> >::iterator it=detidROCsPresent.begin(); it!=detidROCsPresent.end(); ++it){
         uint32_t tempdetid= it->first;
	 std::vector<int> rocs = it->second;
	 unsigned int nrocs = rocs.size();
	 std::map<int,int> mapIPixIndx ;
	 
	 std::vector<double> gainsFromHistos;
	 std::vector<double> pedsFromHistos;

	 CTPPSPixelGainCalibration tempPGCalib;

	 
	 for (unsigned int i = 0; i<nrocs ; i++){
	   getGainsPedsFromHistos(tempdetid,i, rocs[i], pedsFromHistos,gainsFromHistos,mapIPixIndx,nrocs);

	 }

	 //	 if(gainsFromHistos.size()!=nrocs*52*80)
	   //call msg logger
	   //std::cout<<"Something is wrong"<< gainsFromHistos.size()<<std::endl;

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

	 //call msg logger
	 //	 std::cout << "N ROCs = "<<  nrocs<< " N pixels = "<< orderedPeds.size()<< std::endl;
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
WriteCTPPSPixGainCalibrations::getGainsPedsFromHistos(uint32_t detid, int ROCindex, int rocId, std::vector<double> &peds,std::vector<double> &gains, std::map<int,int> & mymap, int nrocs) {
  CTPPSPixelIndices  modulepixels(52*nrocs/2,160);

  std::string tmpslopename = detidHistoNameMap[detid][ROCindex]+"_Slope2D";
  std::string tmpitcpname = detidHistoNameMap[detid][ROCindex]+"_Intercept2D";
  std::string tmpchi2name = detidHistoNameMap[detid][ROCindex] + "_Chisquare2D";
  std::string tmpnpfitsname = detidHistoNameMap[detid][ROCindex] + "_Npfits2D";
  TH2D * tempslope   = (TH2D*) m_inputRootFile->Get(tmpslopename.c_str()); 
  TH2D * tempintrcpt = (TH2D*) m_inputRootFile->Get(tmpitcpname.c_str()); 
  TH2D * tempchi2    = (TH2D*) m_inputRootFile->Get(tmpchi2name.c_str()); 
  TH2D * tempnpfit   = (TH2D*) m_inputRootFile->Get(tmpnpfitsname.c_str()); 
  int ncols = tempslope->GetNbinsX(); 
  int nrows = tempslope->GetNbinsY();
  if (nrows != 80  || ncols != 52 )
    std::cout<<"Something wrong ncols = "<< ncols << " and nrows = " << nrows <<std::endl;
  
  for (int jrow = 0; jrow < nrows ; ++jrow) // when scanning through the 2d histo make sure to avoid underflow bin i or j ==0
    for (int icol = 0 ; icol < ncols ; ++icol){
      double tmpslp  = tempslope->GetBinContent(icol+1,jrow+1); 
      double tmpgain = (tmpslp == 0.0) ? 0.0 : 1.0/tmpslp;
      double tmpped  = tempintrcpt->GetBinContent(icol+1,jrow+1);
      // check for noisy/badly calibrated pixels
      int tmpnpfit = tempnpfit -> GetBinContent(icol+1,jrow+1);
      double tmpchi2  = tempchi2 -> GetBinContent(icol+1,jrow+1);
      // if (tmpnpfit ==0 ){ // dead channel
      // 	std::cout << "I declare this channel dead! tmpped = "<< tmpped <<" tmpgain =" << tmpgain<< " and tmpnpfit =" <<tmpnpfit << " and tmpchi2= "<< tmpchi2<<std::endl;
      // 	std::cout << " Dead Pixel column icol = "<<icol <<" and jrow = "<<jrow <<" Name="<< tmpslopename <<std::endl;
      // 	tmpped = -9999.0;
      // }
      // else if((tmpgain <= -2.0 && tmpnpfit < 7) || tmpnpfit < 4 ){
      // 	std::cout << "I declare this channel noisy! tmpped = "<< tmpped <<" tmpgain=" << tmpgain<< " and tmpnpfit =" <<tmpnpfit << " and tmpchi2= "<< tmpchi2<<std::endl; 
      // 	std::cout << "Noisy Pixel column icol = "<<icol <<" and jrow = "<<jrow <<" Name="<< tmpslopename <<std::endl;
      // 	tmpgain = -9999.0;
      // }
      if (tmpnpfit <3) {
	std::cout << " *** Badly calibrated pixel, NPoints = "<<tmpnpfit << " setting dummy values gain = 0.5 and  ped =20.0 ***" <<std::endl;
	std::cout << " **** Dead Pixel column icol = "<<icol <<" and jrow = "<<jrow <<" Name= "<< tmpslopename <<std::endl; 
	tmpgain =1.0/2.0;
	tmpped = 20.0;
      }	
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
DEFINE_FWK_MODULE(WriteCTPPSPixGainCalibrations);
