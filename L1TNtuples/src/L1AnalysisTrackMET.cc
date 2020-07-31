#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMET.h"

L1Analysis::L1AnalysisTrackMET::L1AnalysisTrackMET()
{
}

L1Analysis::L1AnalysisTrackMET::~L1AnalysisTrackMET()
{

}


void L1Analysis::L1AnalysisTrackMET::SetTrackMET(const edm::Handle<l1t::TkEtMissCollection>  TkEtMiss)
{
  l1extra_.TksumEt = TkEtMiss->etMiss();
}

void L1Analysis::L1AnalysisTrackMET::SetGenMET(const edm::Handle<l1t::TkEtMissCollection> genMET)
{
  l1extra_.gensumEt = genMET->etMiss();
}