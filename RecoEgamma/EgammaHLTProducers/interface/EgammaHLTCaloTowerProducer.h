#ifndef EgammaHLTCaloTowerProducer_h
#define EgammaHLTCaloTowerProducer_h

/** \class EgammaHLTCaloTowerProducer
 *
 * Framework module that produces a collection
 * of calo towers in the region of interest for Egamma HLT reconnstruction,
 * \author M. Sani (UCSD)
 *
 */

#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDefs.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"

#include <string>

namespace edm {
  class ConfigurationDescriptions;
}

class EgammaHLTCaloTowerProducer : public edm::global::EDProducer<> {
public:
  EgammaHLTCaloTowerProducer(const edm::ParameterSet &);
  ~EgammaHLTCaloTowerProducer() override{};
  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);
  void produce(edm::StreamID, edm::Event &, edm::EventSetup const &) const final;

  const edm::EDGetTokenT<CaloTowerCollection> towers_;
  const double cone_;
  const edm::EDGetTokenT<edm::View<reco::Candidate>> l1isoseeds_;
  const edm::EDGetTokenT<edm::View<reco::Candidate>> l1nonisoseeds_;
  const double EtThreshold_;
  const double EThreshold_;
};

#endif
