#ifndef JetMETCorrections_FFTJetObjects_FFTJetCorrectionsTypemap_h
#define JetMETCorrections_FFTJetObjects_FFTJetCorrectionsTypemap_h

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/TrackJet.h"
#include "DataFormats/JetReco/interface/FFTAnyJet.h"

#include "CondFormats/JetMETObjects/interface/FFTJetCorrTypes.h"

template <typename Correction>
struct FFTJetCorrectionsTypemap {};

#define FFTJET_CORRECTIONS_TYPEMAP_ENTRY(fromtype, totype)  \
  template <>                                               \
  struct FFTJetCorrectionsTypemap<fftcorrtypes::fromtype> { \
    typedef reco::FFTAnyJet<reco::totype> jet_type;         \
  }

FFTJET_CORRECTIONS_TYPEMAP_ENTRY(BasicJet, BasicJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(GenJet, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CaloJet, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFJet, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(TrackJet, TrackJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(JPTJet, JPTJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFCHS0, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFCHS1, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFCHS2, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(BasicJetSys, BasicJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(GenJetSys, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CaloJetSys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFJetSys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(TrackJetSys, TrackJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(JPTJetSys, JPTJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFCHS0Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFCHS1Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PFCHS2Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Gen0, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Gen1, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Gen2, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF0, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF1, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF2, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF3, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF4, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo0, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo1, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo2, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo3, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo4, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Gen0Sys, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Gen1Sys, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Gen2Sys, GenJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF0Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF1Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF2Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF3Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF4Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF5Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF6Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF7Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF8Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(PF9Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo0Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo1Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo2Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo3Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo4Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo5Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo6Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo7Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo8Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(Calo9Sys, CaloJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS0Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS1Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS2Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS3Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS4Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS5Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS6Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS7Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS8Sys, PFJet);
FFTJET_CORRECTIONS_TYPEMAP_ENTRY(CHS9Sys, PFJet);

#endif  // JetMETCorrections_FFTJetObjects_FFTJetCorrectionsTypemap_h
