#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

L1Analysis::L1AnalysisTrackJet::L1AnalysisTrackJet()
{
}

L1Analysis::L1AnalysisTrackJet::~L1AnalysisTrackJet()
{

}



void L1Analysis::L1AnalysisTrackJet::SetTrackJet(const edm::Handle< vector<TkJet> > TrackJets, unsigned maxL1Extra)
{

  for (unsigned int i=0; i<TrackJets->size() && l1extra_.nTrackerJets<maxL1Extra; i++){
    if (TrackJets->at(i).pt()>0){
      if(TrackJets->at(i).pt()>30 && abs(TrackJets->at(i).eta())<2.4)
	l1extra_.trackerJetHt += TrackJets->at(i).pt();
      l1extra_.trackerJetEt .push_back(TrackJets->at(i).pt());
      l1extra_.trackerJetEta.push_back(TrackJets->at(i).eta());
      l1extra_.trackerJetPhi.push_back(TrackJets->at(i).phi());
      l1extra_.nTrackerJets++;
    }
  }
}

void L1Analysis::L1AnalysisTrackJet::SetGenJet(const edm::Handle<reco::GenJetCollection> genJets, unsigned maxL1Extra)
{

  reco::GenJetCollection::const_iterator genJetItr = genJets->begin();
  reco::GenJetCollection::const_iterator genJetEnd = genJets->end();
  for( ; genJetItr != genJetEnd ; ++genJetItr) {
    if(genJetItr->pt()>30 && abs(genJetItr->eta())<2.4)
      l1extra_.genJetHt += genJetItr->pt();
    l1extra_.genJetPt.push_back( genJetItr->pt() );
    l1extra_.genJetEta.push_back( genJetItr->eta() );
    l1extra_.genJetPhi.push_back( genJetItr->phi() );
    l1extra_.genJetM.push_back( genJetItr->mass() );
    l1extra_.nGenJet++;
  }
}
