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
      l1extra_.TksumEt.push_back(TkEtMiss->at(i).etMiss());
    if (name == "Cut")
      l1extra_.CutsumEt.push_back(TkEtMiss->at(i).etMiss());
    if (name == "MVA")
      l1extra_.MVAsumEt.push_back(TkEtMiss->at(i).etMiss());
    if (name == "Gen")
      l1extra_.GensumEt.push_back(TkEtMiss->at(i).etMiss());
      
}
}

