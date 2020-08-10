#ifndef __L1Analysis_L1AnalysisTrackJet_H__
#define __L1Analysis_L1AnalysisTrackJet_H__

//-------------------------------------------------------------------------------
// Created 02/03/2010 - A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------


#include "DataFormats/L1TCorrelator/interface/TkJet.h"
#include "DataFormats/L1TCorrelator/interface/TkJetFwd.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackJetDataFormat.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"


namespace L1Analysis
{
  class L1AnalysisTrackJet 
  {
  public:
    L1AnalysisTrackJet();
    ~L1AnalysisTrackJet();
    void Reset() {l1extra_.Reset();}

    // Add new PFJet collections 
    void SetGenJet  (const edm::Handle<reco::GenJetCollection > genJets,    unsigned maxL1Extra);
    void SetTrackJet  (const      edm::Handle<l1t::TkJetCollection>  PFJet,    unsigned maxL1Extra);

    L1AnalysisTrackJetDataFormat * getData() {return &l1extra_;}




  private :
    L1AnalysisTrackJetDataFormat l1extra_;
  }; 
}
#endif