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


  // output file
  edm::Service<TFileService> fs_;

  // tree
  TTree * tree_;

  edm::EDGetTokenT<l1t::TkEtMissCollection> genMET_;
  edm::EDGetTokenT<l1t::TkEtMissCollection> TkMET_;
  edm::EDGetTokenT<l1t::TkEtMissCollection> CutTkMET_;
  edm::EDGetTokenT<l1t::TkEtMissCollection> MVATkMET_;


};

L1TrackMETTreeProducer::L1TrackMETTreeProducer(const edm::ParameterSet& iConfig){

  genMET_ = consumes<l1t::TkEtMissCollection>(iConfig.getParameter<edm::InputTag>("genMETToken"));
  TkMET_  = consumes<l1t::TkEtMissCollection>(iConfig.getParameter<edm::InputTag>("TkMETToken"));
  CutTkMET_  = consumes<l1t::TkEtMissCollection>(iConfig.getParameter<edm::InputTag>("CutTkMETToken"));
  MVATkMET_  = consumes<l1t::TkEtMissCollection>(iConfig.getParameter<edm::InputTag>("MVATkMETToken"));

  l1Extra     = new L1Analysis::L1AnalysisTrackMET();
  l1ExtraData = l1Extra->getData();

  // set up output
  tree_=fs_->make<TTree>("L1TrkMETTree", "L1TrkMETTree");
  tree_->Branch("L1TrkMET", "L1Analysis::L1AnalysisTrackMETDataFormat", &l1ExtraData, 32000, 3);

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


  edm::Handle<l1t::TkEtMissCollection> genMET;
  iEvent.getByToken(genMET_, genMET);

  if (genMET.isValid()){ 
    l1Extra->SetTrackMET(genMET,"Gen");
  } else {
    edm::LogWarning("MissingProduct") << "Gen MET not found. Branch will not be filled" << std::endl;
  }


  edm::Handle<l1t::TkEtMissCollection> TkMET;
  iEvent.getByToken(TkMET_,TkMET);


  if (TkMET.isValid()){
    l1Extra->SetTrackMET(TkMET,"Track");
  } else {
    edm::LogWarning("MissingProduct") << "L1 Track MET not found. Branch will not be filled" << std::endl;
  }

  edm::Handle<l1t::TkEtMissCollection> CutTkMET;
  iEvent.getByToken(CutTkMET_,CutTkMET);


  if (CutTkMET.isValid()){
    l1Extra->SetTrackMET(CutTkMET,"Cut");
  } else {
    edm::LogWarning("MissingProduct") << "L1 Track Cut MET not found. Branch will not be filled" << std::endl;
  }

  edm::Handle<l1t::TkEtMissCollection> MVATkMET;
  iEvent.getByToken(MVATkMET_,MVATkMET);


  if (MVATkMET.isValid()){
    l1Extra->SetTrackMET(MVATkMET,"MVA");
  } else {
    edm::LogWarning("MissingProduct") << "L1 Track MVA MET not found. Branch will not be filled" << std::endl;
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