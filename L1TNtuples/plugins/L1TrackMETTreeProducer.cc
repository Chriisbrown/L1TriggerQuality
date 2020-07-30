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

#include "DataFormats/L1TCorrelator/interface/TkEtMiss.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMissFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"


// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMET.h"

//
// class declaration
//

class L1TrackMETTreeProducer : public edm::EDAnalyzer {
public:
  explicit L1TrackMETTreeProducer(const edm::ParameterSet&);
  ~L1TrackMETTreeProducer();


private:
  virtual void beginJob(void) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

public:

  L1Analysis::L1AnalysisTrackMET* l1Extra;
  L1Analysis::L1AnalysisTrackMETDataFormat * l1ExtraData;

private:

  unsigned maxL1Extra_;

  // output file
  edm::Service<TFileService> fs_;

  // tree
  TTree * tree_;

  edm::EDGetTokenT<reco::GenMET > genMET_;
  edm::EDGetTokenT<l1t::TkEtMiss> TkMET_;


};

L1TrackMETTreeProducer::L1TrackMETTreeProducer(const edm::ParameterSet& iConfig){

  genMET_ = consumes<reco::GenMET>(iConfig.getUntrackedParameter<edm::InputTag>("genMETToken"));
  TkMET_ = consumes<l1t::TkEtMiss > (iConfig.getParameter<edm::InputTag>("TkMETToken"));


  maxL1Extra_ = iConfig.getParameter<unsigned int>("maxL1Extra");

  l1Extra     = new L1Analysis::L1AnalysisTrackMET();
  l1ExtraData = l1Extra->getData();

  // set up output
  tree_=fs_->make<TTree>("L1METTree", "L1METTree");
  tree_->Branch("L1MET", "L1Analysis::L1AnalysisTrackMETDataFormat", &l1ExtraData, 32000, 3);

}


L1TrackMETTreeProducer::~L1TrackMETTreeProducer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
L1TrackMETTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

 l1Extra->Reset();


  edm::Handle<reco::GenMET> genMET;
  iEvent.getByToken(genMET_, genMET);

  if (genMET.isValid()){ 
    l1Extra->SetGenMET(genMET,maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "Gen MET not found. Branch will not be filled" << std::endl;
  }


  edm::Handle<l1t::TkEtMiss> TkMET;
  iEvent.getByToken(TkMET_,TkMET);


  if (TkMET.isValid()){
    l1Extra->SetTrackMET(TkMET, maxL1Extra_);
  } else {
    edm::LogWarning("MissingProduct") << "L1 Track MET not found. Branch will not be filled" << std::endl;
  }


  tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
L1TrackMETTreeProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TrackMETTreeProducer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TrackMETTreeProducer);