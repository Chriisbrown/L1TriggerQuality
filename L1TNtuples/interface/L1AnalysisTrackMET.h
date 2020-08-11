#ifndef __L1Analysis_L1AnalysisTrackMET_H__
#define __L1Analysis_L1AnalysisTrackMET_H__

//-------------------------------------------------------------------------------
// Created 02/03/2010 - A.C. Le Bihan
// 
// 
// Original code : UserCode/L1TriggerDPG/L1ExtraTreeProducer - Jim Brooke
//-------------------------------------------------------------------------------

#include "DataFormats/L1TCorrelator/interface/TkEtMiss.h"
#include "DataFormats/L1TCorrelator/interface/TkEtMissFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMETDataFormat.h"

namespace L1Analysis
{
  class L1AnalysisTrackMET 
  {
  public:
    L1AnalysisTrackMET();
    ~L1AnalysisTrackMET();
    void Reset() {l1extra_.Reset();}

    // Add new MET collections 
    void SetTrackMET  (const edm::Handle<l1t::TkEtMissCollection >  MET, std::string name);
    void SetGenMET (const edm::Handle<reco::GenMETCollection >  MET);

    L1AnalysisTrackMETDataFormat * getData() {return &l1extra_;}

  private :
    L1AnalysisTrackMETDataFormat l1extra_;
  }; 
}
#endif

