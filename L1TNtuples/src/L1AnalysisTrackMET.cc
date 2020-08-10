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
      l1extra_.TrkMet = (TkEtMiss->at(0).etMiss());
    if (name == "Cut")
      l1extra_.CutTrkMet = (TkEtMiss->at(0).etMiss());
    if (name == "MVA")
      l1extra_.MVATrkMet = (TkEtMiss->at(0).etMiss());

}


void L1Analysis::L1AnalysisTrackMET::SetGenMET(const edm::Handle<reco::GenMETCollection> genMetsTrue)
{
  l1extra_->genMet = genMetsTrue->at(0).pt();

}

