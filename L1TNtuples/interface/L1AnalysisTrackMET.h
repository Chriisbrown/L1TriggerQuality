#ifndef __L1Analysis_L1AnalysisTrackMET_H__
#define __L1Analysis_L1AnalysisTrackMET_H__

//-------------------------------------------------------------------------------
// Created 02/03/2010 - A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------

#include "DataFormats/L1TCorrelator/interface/TkEtMiss.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMETDataFormat.h"

namespace L1Analysis
{
  class L1AnalysisTrackMET 
  {
  public:
    L1AnalysisTrackMET();
    ~L1AnalysisTrackMET();
    void Reset() {l1extra_.Reset();}

    // Add new PFJet collections 
    void SetGenMET  (const edm::Handle<reco::GenMET > genMET,    unsigned maxL1Extra);
    void SetTrackMET  (const edm::Handle< l1t::TkEtMiss >  TkEtMiss,    unsigned maxL1Extra);

    L1AnalysisTrackMETDataFormat * getData() {return &l1extra_;}




  private :
    L1AnalysisTrackMETDataFormat l1extra_;
  }; 
}
#endif

