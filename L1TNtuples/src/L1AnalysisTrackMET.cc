#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMET.h"

L1Analysis::L1AnalysisTrackMET::L1AnalysisTrackMET()
{
}

L1Analysis::L1AnalysisTrackMET::~L1AnalysisTrackMET()
{

}


void L1Analysis::L1AnalysisTrackMET::SetTrackMET(const edm::Handle<l1t::TkEtMissCollection>  TkEtMiss)
{

  for (unsigned int i=0; i<TkEtMiss->size(); i++){
      l1extra_.TksumEt.push_back(TkEtMiss->at(i).etMiss());
      
}
}

void L1Analysis::L1AnalysisTrackMET::SetGenMET(const edm::Handle<l1t::TkEtMissCollection> genMET)
{

  for (unsigned int i=0; i<genMET->size(); i++){
      l1extra_.gensumEt.push_back(genMET->at(i).etMiss());
      
}
}