#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CondTools/CTPPS/interface/CTPPSOpticsParamRootFileSourceHandler.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef popcon::PopConAnalyzer<CTPPSOpticsParamRootFileSourceHandler> CTPPSOpticsParamRootFilePopConAnalyzer;
//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSOpticsParamRootFilePopConAnalyzer);
