#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMET.h"

L1Analysis::L1AnalysisTrackMET::L1AnalysisTrackMET()
{
}

L1Analysis::L1AnalysisTrackMET::~L1AnalysisTrackMET()
{

}


void L1Analysis::L1AnalysisTrackMET::SetTrackMET(const edm::Handle<l1t::TkEtMissCollection>  TkEtMiss,std::string name)
{

    if (name == "Track")
    {
      l1extra_.TrkMet =   (TkEtMiss->at(0).etMiss());
      l1extra_.TrkSumEt = (TkEtMiss->at(0).etTotal());
      l1extra_.TrkMetPhi =(TkEtMiss->at(0).phi());
    }
    if (name == "Cut")
    {
      l1extra_.CutTrkMet =    (TkEtMiss->at(0).etMiss());
      l1extra_.CutTrkSumEt =  (TkEtMiss->at(0).etTotal());
      l1extra_.CutTrkMetPhi = (TkEtMiss->at(0).phi());
    }
    if (name == "MVA")
    {
      l1extra_.MVATrkMet =   (TkEtMiss->at(0).etMiss());
      l1extra_.MVATrkSumEt = (TkEtMiss->at(0).etTotal());
      l1extra_.MVATrkMetPhi =(TkEtMiss->at(0).phi());
    }

}


void L1Analysis::L1AnalysisTrackMET::SetGenMET(const edm::Handle<reco::GenMETCollection> genMetsTrue)
{
  l1extra_.GenSumEt  = genMetsTrue->at(0).sumEt();
  l1extra_.GenMetPhi = genMetsTrue->at(0).phi();
  l1extra_.GenMet    = genMetsTrue->at(0).pt();
}

