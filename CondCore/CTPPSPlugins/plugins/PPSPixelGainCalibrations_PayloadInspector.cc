// Author C.Mora 
// October 2020 
// clemencia.mora.herrera@cern.ch

#include "CondCore/Utilities/interface/PayloadInspectorModule.h"
#include "CondCore/Utilities/interface/PayloadInspector.h"
#include "CondCore/CondDB/interface/Time.h"


// the data format of the condition to be inspected
#include "CondFormats/PPSObjects/interface/CTPPSPixelGainCalibrations.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
//#include "CondCore/EcalPlugins/plugins/EcalDrawUtils.h"

#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLine.h"
#include "TLatex.h"

#include <memory>
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

	  const CTPPSPixelGainCalibrations::CalibMap& mymap = payload->getCalibMap();  //just to get the keys?
	  
	  for (CTPPSPixelGainCalibrations::CalibMap::const_iterator it = mymap.begin(); it != mymap.end(); ++it) {
	    uint32_t detId = it->first;
	    detids.push_back(detId);

	    CTPPSPixelDetId pixelPlaneId=CTPPSDetId(detId);
	    
	    //   ss << "Address of  detId = " << (&detId) << " and of it = " << (&it) << " and of it->first = " << (&(it->first));
	    
	    ss << "Content  of payload for key: pixelPlaneId= " << pixelPlaneId <<"  \n";
	   
	    const CTPPSPixelGainCalibration& mycalibs = it->second;
	    //TH2D mypeds(namep.c_str(), tlp.c_str(), 156, 0., 156., 160, 0., 160.);
	    //TH2D mygains(nameg.c_str(), tlg.c_str(), 156, 0., 156., 160, 0., 160.);
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
  
  
  
  /* plot image of pps pixel pedestals 1d plot */
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
	  
	  //CTPPSPixelDetId pixelPlaneId=CTPPSDetId(detId);
	  
	  
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
  
  
}//namespace


// Register the classes as boost python plugin
PAYLOAD_INSPECTOR_MODULE(PPSPixelGainCalibrations) {
  PAYLOAD_INSPECTOR_CLASS(PPSPixelGainCalibrationsPedestalHist);
  PAYLOAD_INSPECTOR_CLASS(PPSPixelGainCalibrationsPedestalsPlot);
  //  PAYLOAD_INSPECTOR_CLASS(PPSPixelGainCalibrationsDiff);
  //  PAYLOAD_INSPECtor_CLASS(PPSPixelGainCalibrationsSummaryPlot);
}
