// Author  C.Mora 
// October 2020 
// clemencia.mora.herrera@cern.ch

#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspector.h"
#include "CondCore/CondDB/interface/Time.h"


// the data format of the condition to be inspected
#include "CondFormats/PPSObjects/interface/CTPPSPixelGainCalibrations.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"

#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "TLatex.h"

#include <memory>
#include <cmath>
#include <sstream>

namespace {
  class PPSPixelGainCalibrationsPedestalHist : public cond::payloadInspector::Histogram1D<CTPPSPixelGainCalibrations> {
    
  public:
    PPSPixelGainCalibrationsPedestalHist()
      : cond::payloadInspector::Histogram1D<CTPPSPixelGainCalibrations>("PPS Pixel Pedestals test", "PPS Pixel Pedestals test", 120,-10.0, 50.0)
    {
      Base::setSingleIov(true);
    }
    
    bool fill() override{

      auto tag = PlotBase::getTag<0>();
      for (auto const& iov : tag.iovs) {
	std::shared_ptr<CTPPSPixelGainCalibrations> payload = Base::fetchPayload(std::get<1>(iov));
        if (payload.get()) {
	  
	  std::stringstream ss;
          ss << "Summary of PPS pixel pedestals:" << std::endl;
	  
	  
	  std::vector<uint32_t> detids;

	  const CTPPSPixelGainCalibrations::CalibMap& mymap = payload->getCalibMap();
	  
	  for (CTPPSPixelGainCalibrations::CalibMap::const_iterator it = mymap.begin(); it != mymap.end(); ++it) {
	    uint32_t detId = it->first;
	    detids.push_back(detId);

	    CTPPSPixelDetId pixelPlaneId=CTPPSDetId(detId);
	    
	    ss << "Content  of payload for key: pixelPlaneId= " << pixelPlaneId <<"  \n";
	   
	    const CTPPSPixelGainCalibration& mycalibs = it->second;
	    int ncols = mycalibs.getNCols();
	    //	    int npix = mycalibs.getIEnd();
	    int nrows = mycalibs.getNRows();  //should be == 160
	    for (int jrow = 0; jrow < nrows; ++jrow)
	      for (int icol = 0; icol < ncols; ++icol) {
		double pedestal = mycalibs.getPed(icol,jrow);
		fillWithValue(pedestal);
		//	if(jrow==0 && icol ==0) ss<<"Pedestal value for pix "<<icol<<" " <<jrow<<" = "<< pedestal <<" \n";
	      }
	    
	    std::cout << ss.str() << std::endl;
	  } //detid	    
	}   // payload
      }     // iovs
      return true;
    }//fill
    
  };//class PPSPixelGainCalibrationsPedestalHist
  
  
  
  /* plot image of pps pixel pedestals 1d plot By plane */
  class PPSPixelGainCalibrationsPedestalsPlot : public cond::payloadInspector::PlotImage<CTPPSPixelGainCalibrations> {
    
  public:
    PPSPixelGainCalibrationsPedestalsPlot() 
      : cond::payloadInspector::PlotImage<CTPPSPixelGainCalibrations>("PPS Pedestal hist") 
    { 
      setSingleIov(true);
    }

    bool fill(const std::vector<std::tuple<cond::Time_t, cond::Hash> >& iovs) override {

      auto iov = iovs.front();     
      std::shared_ptr<CTPPSPixelGainCalibrations> payload = fetchPayload(std::get<1>(iov)); 
      TH1F* histo = new TH1F("allpedestals","allpedestals; Pedestal Value ; Number of pixels  ",120,-10.,50.) ;
      if (payload.get()) {
	
	std::vector<uint32_t> detids;
	
	const CTPPSPixelGainCalibrations::CalibMap& mymap = payload->getCalibMap();  //just to get the keys?
	
	for (CTPPSPixelGainCalibrations::CalibMap::const_iterator it = mymap.begin(); it != mymap.end(); ++it) {
	  uint32_t detId = it->first;
	  detids.push_back(detId);
	  
	  
	  const CTPPSPixelGainCalibration& mycalibs = it->second;
	  int ncols = mycalibs.getNCols();
	  //	    int npix = mycalibs.getIEnd();
	  int nrows = mycalibs.getNRows();  //should be == 160
	  for (int jrow = 0; jrow < nrows; ++jrow)
	    for (int icol = 0; icol < ncols; ++icol) {
	      double pedestal = mycalibs.getPed(icol,jrow);
	      histo->Fill(pedestal);
	    }
	  
	} //detid
      }  // if( payload.get())
      else
	return false;
      //}// iovs
      
      gStyle->SetOptStat(0);
      //set the background color to white                                                                                                                                         
      gStyle->SetFillColor(10);
      gStyle->SetFrameFillColor(10);
      gStyle->SetCanvasColor(10);
      gStyle->SetPadColor(10);
      gStyle->SetTitleFillColor(0);
      gStyle->SetStatColor(10);
      //dont put a colored frame around the plots                                                                                                                                 
      gStyle->SetFrameBorderMode(0);
      gStyle->SetCanvasBorderMode(0);
      gStyle->SetPadBorderMode(0);
      //use the primary color palette                                                                                                                                             
      gStyle->SetPalette(1);
      TCanvas c1("c1", "c1", 1200, 700);
      c1.SetGridx(1);
      c1.SetGridy(1);
      
      c1.cd();
      histo->Draw();
      std::string ImageName("histo1.png");
      c1.SaveAs(ImageName.c_str());
      return true;
      
    }     //end fill() 
    
  }; // class PPSPixelGainCalibrationsPedestalsPlot
  


  /* Image 2D plots pedestals diff by plane */

  class PPSPixelGainCalibrationsPedestalsDiff : public cond::payloadInspector::PlotImage<CTPPSPixelGainCalibrations,cond::payloadInspector::SINGLE_IOV ,2 > {
  public:
    PPSPixelGainCalibrationsPedestalsDiff() : cond::payloadInspector::PlotImage<CTPPSPixelGainCalibrations,cond::payloadInspector::SINGLE_IOV,2>("Diff of 2D histograms for pixel pedestals"){}

    bool fill() override  {

      // trick to deal with the multi-ioved tag and two tag case at the same time
      auto tag1iovs = cond::payloadInspector::PlotBase::getTag<0>().iovs;
      auto tagname1 = cond::payloadInspector::PlotBase::getTag<0>().name;
  
      auto tag2iovs = cond::payloadInspector::PlotBase::getTag<1>().iovs;
      auto tagname2 = cond::payloadInspector::PlotBase::getTag<1>().name;
  
      auto iov0 = tag1iovs.front();
      auto iov1 = tag2iovs.front();

      std::string namep=("pedestals_iov_");
      // namep0=namep0+std::to_string(std::get<0>(iov0));
      // std::string namep1("pedestals_iov_");
      // namep1=namep1+std::to_string(std::get<0>(iov1));

      // std::cout << namep0 << std::endl;
      // std::cout << namep1 << std::endl;

      std::string tlp(" ; column; row; diff ");
      // std::string tlp1(namep1+"; column; row; diff");
      // std::cout << tlp0 << std::endl;
      // std::cout << tlp1 << std::endl;
      TH2D** peds0= new TH2D*[30];
      TH2D** peds1= new TH2D*[30];

      std::shared_ptr<CTPPSPixelGainCalibrations> payload0 = fetchPayload(std::get<1>(iov0)); 
      std::shared_ptr<CTPPSPixelGainCalibrations> payload1 = fetchPayload(std::get<1>(iov1)); 
      int kPlanes =  0;
      std::vector<uint32_t> detids;
      std::vector<std::string> histNames;	

      if (payload0.get() && payload1.get() ) {
	
	const CTPPSPixelGainCalibrations::CalibMap& mymap0 = payload0->getCalibMap();  
	const CTPPSPixelGainCalibrations::CalibMap& mymap1_const = payload1->getCalibMap();  
	
	for (CTPPSPixelGainCalibrations::CalibMap::const_iterator it = mymap0.begin(); it != mymap0.end(); ++it) {
	  uint32_t detId = it->first;
	  detids.push_back(detId);

	  CTPPSPixelDetId pixelPlaneId=CTPPSDetId(detId);
	  std::string planeName ("Arm_"+  std::to_string(pixelPlaneId.arm())
				 + "_Station_" + std::to_string(pixelPlaneId.station()) 
				 + "_RP_" + std::to_string(pixelPlaneId.rp()) 
				 + "_plane_"+ std::to_string(pixelPlaneId.plane()));
	  histNames.push_back(planeName);
	  std::cout<<planeName<<std::endl;

	}// loop over detIds
	kPlanes =  detids.size();
	std::cout<<"Here kPlanes =" << kPlanes <<std::endl;
	if(kPlanes>30){
	  std::cout<< "The array is not big enough" <<std::endl;
	  return false;
	}
	for (int i=0; i< kPlanes ; ++i ){
	  uint32_t plane_i = detids.at(i);
	  std::string namep0(namep+"_"+std::to_string(std::get<0>(iov0))+"_" + histNames.at(i));
	  std::string namep1(namep+"_"+std::to_string(std::get<0>(iov1))+"_" + histNames.at(i));
	  peds0[i]=new TH2D(namep0.c_str(), tlp.c_str(), 156, 0., 156., 160, 0., 160.);
	  peds1[i]=new TH2D(namep1.c_str(), tlp.c_str(), 156, 0., 156., 160, 0., 160.);

	  int ncols0  =  mymap0.at(plane_i).getNCols();
	  int npix0  =  mymap0.at(plane_i).getIEnd();
	  int nrows0  =  mymap0.at(plane_i).getNRows();
	  std::cout<< "npix map 0 " << npix0 << std::endl;
	  for (int jrow=0; jrow < nrows0 ; ++jrow)
	    for(int icol=0; icol < ncols0 ; ++icol)
	     peds0[i]->Fill(icol,jrow,mymap0.at(plane_i).getPed(icol,jrow));
	  // loop over pixels of a plane in iov0

	  CTPPSPixelGainCalibrations::CalibMap mymap1;
	  CTPPSPixelGainCalibration pgc;
	  try{
	    pgc = mymap1_const.at(plane_i);
	  } 
	  catch (const std::out_of_range& oor) {
	    std::cerr << "Out of Range error: " << oor.what() << '\n';
	    std::cout << "No calibrations for plane "<<histNames.at(i) << " in iov "<<std::to_string(std::get<0>(iov1)) << std::endl;
	    pgc = CTPPSPixelGainCalibration();
	    // if the plane doesn't exist in second iov, use empty calibs objects
	  }
	  mymap1.emplace(plane_i,pgc);
	  
	  int ncols1  =  mymap1.at(plane_i).getNCols();
	  int npix1  =  mymap1.at(plane_i).getIEnd();
	  int nrows1  =  mymap1.at(plane_i).getNRows();
	  std::cout<< "npix map 1 " << npix1 << std::endl;
	  for (int jrow=0; jrow < nrows1 ; ++jrow)
	    for(int icol=0; icol < ncols1 ; ++icol)
	     peds1[i]->Fill(icol,jrow,mymap1.at(plane_i).getPed(icol,jrow));  
	  // loop over pixels of a plane in iov1

	}// loop over planes 
      }//if payloads

      gStyle->SetOptStat(0);
      //set the background color to white                                                                                                                                         
      gStyle->SetFillColor(10);
      gStyle->SetFrameFillColor(10);
      gStyle->SetCanvasColor(10);
      gStyle->SetPadColor(10);
      gStyle->SetTitleFillColor(0);
      gStyle->SetStatColor(10);
      //dont put a colored frame around the plots                                                                                                                                 
      gStyle->SetFrameBorderMode(0);
      gStyle->SetCanvasBorderMode(0);
      gStyle->SetPadBorderMode(0);
      //use the primary color palette                                                                                                                                             
      gStyle-> SetPalette(1);
      gStyle-> SetPadRightMargin(0.15);
      int padrows = int(floor(kPlanes/3));
      std::cout<<padrows<<std::endl;
      TCanvas c1("c1","Pedestals difference",1500,300*padrows);
      c1.Divide(3,padrows);
      c1.SetRightMargin(0.05);
      for(int k = 0; k <kPlanes ; ++k){
	c1.cd(k+1);
	peds0[k]->Add(peds1[k], -1.0);
	std::string padname("Pedestals_"+histNames[k]+"_diff_iov0_"+std::to_string(std::get<0>(iov0))+"_iov1_"+std::to_string(std::get<0>(iov1))) ;
	peds0[k]->SetTitle(padname.c_str());
	peds0[k]->SetTitleSize(14);
	peds0[k]->Draw("colz");
      }
      std::string ImgName("PedestalsComparison.png");
      c1.SaveAs(ImgName.c_str());
      return true;
    }//fill
  }; //class PPSPixelGainCalibrationsPedestalsDiff
}//namespace


// Register the classes as boost python plugin
PAYLOAD_INSPECTOR_MODULE(PPSPixelGainCalibrations) {
  PAYLOAD_INSPECTOR_CLASS(PPSPixelGainCalibrationsPedestalHist);
  PAYLOAD_INSPECTOR_CLASS(PPSPixelGainCalibrationsPedestalsPlot);
  PAYLOAD_INSPECTOR_CLASS(PPSPixelGainCalibrationsPedestalsDiff);
  //  PAYLOAD_INSPECtor_CLASS(PPSPixelGainCalibrationsSummaryPlot);
}
