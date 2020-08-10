// -*- C++ -*-
//
// Package:    UserCode/L1TriggerDPG
// Class:      L1PhaseIPFJetTreeProducer
// 
/**\class L1PhaseIPFJetTreeProducer L1PhaseIPFJetTreeProducer.cc UserCode/L1TriggerDPG/src/L1PhaseIPFJetTreeProducer.cc
   Description: Produce L1 Extra tree
   Implementation:
*/
//
// Original Author:  Alex Tapper
//         Created:  
// $Id: L1PhaseIPFJetTreeProducer.cc,v 1.5 2013/01/06 21:55:55 jbrooke Exp $
//
//


// system include files
#include <memory>

// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// data formats
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/L1TCorrelator/interface/TkJet.h"
#include "DataFormats/L1TCorrelator/interface/TkJetFwd.h"

// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackJet.h"

//
// class declaration
//

class L1TrackJetTreeProducer : public edm::EDAnalyzer {
public:
  explicit L1TrackJetTreeProducer(const edm::ParameterSet&);
  ~L1TrackJetTreeProducer();


private:
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

public:

  L1Analysis::L1AnalysisTrackJet* l1Extra;
  L1Analysis::L1AnalysisTrackJetDataFormat * l1ExtraData;

private:

  unsigned maxL1Extra_;

  // output file
  edm::Service<TFileService> fs_;

  // tree
  TTree * tree_;

  edm::EDGetTokenT<reco::GenJetCollection> genJets_;
  edm::EDGetTokenT<std::vector<l1t::TkJet>> TrackJets_;

};

L1TrackJetTreeProducer::L1TrackJetTreeProducer(const edm::ParameterSet& iConfig){

  genJets_ = consumes<reco::GenJetCollection>(iConfig.getUntrackedParameter<edm::InputTag>("genJetToken"));
  TrackJets_ = consumes<std::vector<l1t::TkJet> > (iConfig.getParameter<edm::InputTag>("l1TrackJets"));

  maxL1Extra_ = iConfig.getParameter<unsigned int>("maxL1Extra");

  l1Extra     = new L1Analysis::L1AnalysisTrackJet();
  l1ExtraData = l1Extra->getData();

  // set up output
  tree_=fs_->make<TTree>("L1TrackJetTree", "L1TrackJetTree");
  tree_->Branch("L1TrackJet", "L1Analysis::L1AnalysisTrackJetDataFormat", &l1ExtraData, 32000, 3);

}


L1TrackJetTreeProducer::~L1TrackJetTreeProducer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
L1TrackJetTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

 l1Extra->Reset();


  edm::Handle<reco::GenJetCollection> genJets;
  iEvent.getByToken(genJets_, genJets);

  if (genJets.isValid()){ 
    l1Extra->SetGenJet(genJets,maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "Gen jets not found. Branch will not be filled" << std::endl;
  }





  edm::Handle<std::vector<l1t::TkJet>> TrackJets;
  iEvent.getByToken(TrackJets_,TrackJets);
  if (TrackJets.isValid()){
    l1Extra->SetTrackJet(TrackJets, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "TrackJet Jet not found. Branch will not be filled" << std::endl;
  }




  tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
L1TrackJetTreeProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TrackJetTreeProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TrackJetTreeProducer);