#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

L1Analysis::L1AnalysisTrackJet::L1AnalysisTrackJet()
{
}

L1Analysis::L1AnalysisTrackJet::~L1AnalysisTrackJet()
{

}



void L1Analysis::L1AnalysisTrackJet::SetTrackJet(const edm::Handle< vector<l1t::TkJet> > TrackJets, unsigned maxL1Extra,string name)
{
  if (name == "None")
  {

    for (unsigned int i=0; i<TrackJets->size() && l1extra_.nTrkJets<maxL1Extra; i++){
      if (TrackJets->at(i).pt()>0){
        if(TrackJets->at(i).pt()>30 && abs(TrackJets->at(i).eta())<2.4)
    l1extra_.TrkJetHt += TrackJets->at(i).pt();
        l1extra_.TrkJetEt .push_back(TrackJets->at(i).pt());
        l1extra_.TrkJetEta.push_back(TrackJets->at(i).eta());
        l1extra_.TrkJetPhi.push_back(TrackJets->at(i).phi());
        l1extra_.nTrkJets++;
      }
    }
  }

  if (name == "Cut")
  {

    for (unsigned int i=0; i<TrackJets->size() && l1extra_.nCutTrkJets<maxL1Extra; i++){
      if (TrackJets->at(i).pt()>0){
        if(TrackJets->at(i).pt()>30 && abs(TrackJets->at(i).eta())<2.4)
    l1extra_.CutTrkJetHt += TrackJets->at(i).pt();
        l1extra_.CutTrkJetEt .push_back(TrackJets->at(i).pt());
        l1extra_.CutTrkJetEta.push_back(TrackJets->at(i).eta());
        l1extra_.CutTrkJetPhi.push_back(TrackJets->at(i).phi());
        l1extra_.nCutTrkJets++;
      }
    }
  }

  if (name == "MVA")
  {

    for (unsigned int i=0; i<TrackJets->size() && l1extra_.nMVATrkJets<maxL1Extra; i++){
      if (TrackJets->at(i).pt()>0){
        if(TrackJets->at(i).pt()>30 && abs(TrackJets->at(i).eta())<2.4)
    l1extra_.MVATrkJetHt += TrackJets->at(i).pt();
        l1extra_.MVATrkJetEt .push_back(TrackJets->at(i).pt());
        l1extra_.MVATrkJetEta.push_back(TrackJets->at(i).eta());
        l1extra_.MVATrkJetPhi.push_back(TrackJets->at(i).phi());
        l1extra_.nMVATrkJets++;
      }
    }
  }
}

void L1Analysis::L1AnalysisTrackJet::SetGenJet(const edm::Handle<reco::GenJetCollection> genJets, unsigned maxL1Extra)
{

  reco::GenJetCollection::const_iterator genJetItr = genJets->begin();
  reco::GenJetCollection::const_iterator genJetEnd = genJets->end();
  for( ; genJetItr != genJetEnd ; ++genJetItr) {
    if(genJetItr->pt()>30 && abs(genJetItr->eta())<2.4)
      l1extra_.GenJetHt += genJetItr->pt();
    l1extra_.GenJetPt.push_back( genJetItr->pt() );
    l1extra_.GenJetEta.push_back( genJetItr->eta() );
    l1extra_.GenJetPhi.push_back( genJetItr->phi() );
    l1extra_.GenJetM.push_back( genJetItr->mass() );
    l1extra_.nGenJet++;
  }
}
