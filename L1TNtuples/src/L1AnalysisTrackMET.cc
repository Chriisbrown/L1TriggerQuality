#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMET.h"

L1Analysis::L1AnalysisTrackMET::L1AnalysisTrackMET()
{
}

L1Analysis::L1AnalysisTrackMET::~L1AnalysisTrackMET()
{

}


void L1Analysis::L1AnalysisTrackMET::SetTrackMET(const edm::Handle<l1t::TkEtMissCollection>  TkEtMiss,std::string name)
{

  for (unsigned int i=0; i<TkEtMiss->size(); i++){
    if (name == "Track")
<<<<<<< HEAD
      l1extra_.TrkMet.push_back(TkEtMiss->at(i).etMiss());
    if (name == "Cut")
      l1extra_.CutTrkMet.push_back(TkEtMiss->at(i).etMiss());
    if (name == "MVA")
      l1extra_.MVATrkMet.push_back(TkEtMiss->at(i).etMiss());
    if (name == "Gen")
      l1extra_.GenMet.push_back(TkEtMiss->at(i).etMiss());
=======
      l1extra_.TrkMet = (TkEtMiss->at(i).etMiss());
    if (name == "Cut")
      l1extra_.CutTrkMet = (TkEtMiss->at(i).etMiss());
    if (name == "MVA")
      l1extra_.MVATrkMet = (TkEtMiss->at(i).etMiss());
    if (name == "Gen")
      l1extra_.GenMet = (TkEtMiss->at(i).etMiss());
>>>>>>> ae50c0cdf3e431b3f8f93276e4d768ba45d026cc
      
}
}

