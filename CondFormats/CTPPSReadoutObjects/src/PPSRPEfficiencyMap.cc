#include "CondFormats/CTPPSReadoutObjects/interface/PPSRPEfficiencyMap.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TFile.h"

//Constructor only with number of bins
PPSRPEfficiencyMap::PPSRPEfficiencyMap(const uint32_t & nxbins, const uint32_t & nybins){
  m_nBins = (nxbins+2)*(nybins+2); // add 2 for overflow and underflow
  m_efficiencytable.resize(m_nBins); // resized but empty
}

//Constructor from a 2D histogram
PPSRPEfficiencyMap::PPSRPEfficiencyMap(const TH2D & effh ){
  FillPPSRPEfficiencyMap(effh);
}

void PPSRPEfficiencyMap::setEffEntry(const uint32_t &x,const uint32_t & y, const double & eff){
  unsigned int ib = y*m_nBinsX +x ;
  double xpos = m_xLow + (x-0.5)*m_xWidth;
  double ypos = m_yLow + (y-0.5)*m_yWidth;
  effentry meff = {x,y,xpos,ypos,eff};

  if (ib >= m_efficiencytable.size())
    m_efficiencytable.resize(ib+1);
  m_efficiencytable[ib] = meff;
    
}

PPSRPEfficiencyMap::PPSRPEfficiencyMap(const std::string &filename , const std::string &histopath){
  TFile *infile = TFile::Open(filename.c_str());
  if (infile == nullptr)
    throw cms::Exception("PPSRPEfficnecyMap")<< "Cannot open file " << filename << "."; 
  auto histoeff = dynamic_cast<TH2D*>( infile->Get( histopath.c_str() ) );
  FillPPSRPEfficiencyMap(*histoeff);
  delete infile;
  
}


void PPSRPEfficiencyMap::FillPPSRPEfficiencyMap(const TH2D & effh ){
  m_nBins = effh.GetNcells();
  m_nBinsX = effh.GetNbinsX();
  m_nBinsY = effh.GetNbinsY();
  
  const TAxis* ax = effh.GetXaxis();
  const TAxis* ay = effh.GetYaxis();
  m_xLow   = ax->GetXmin();
  m_xWidth = ax->GetBinWidth(1);

  m_yLow   = ay->GetXmin();
  m_yWidth = ay->GetBinWidth(1);
  
  for(int ib = 0 ; ib<m_nBins ; ib++){
    int nx = m_nBinsX+2;
    int ny = m_nBinsY+2;

    uint32_t ix = ib%nx;
    uint32_t iy = ((ib-ix)/nx)%ny;

    double xpos = ax->GetBinCenter(ix);
    double ypos = ay->GetBinCenter(iy);

    double effc=effh.GetBinContent(ib);

    effentry meff = {ix,iy,xpos,ypos,effc};
    m_efficiencytable.push_back(meff);
    
  }
}

