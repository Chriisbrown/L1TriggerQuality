#include "L1Trigger/L1TNtuples/interface/L1AnalysisTrackMETDataFormat.h"
#include "DataFormats/METReco/interface/GenMET.h"

L1Analysis::L1AnalysisTrackMET::L1AnalysisTrackMET()
{
}

L1Analysis::L1AnalysisTrackMET::~L1AnalysisTrackMET()
{

}


void L1Analysis::L1AnalysisTrackMET::SetTrackMET(const edm::Handle< l1t::TkEtMiss >  TkEtMiss, unsigned maxL1Extra)
{
  l1extra_.TksumEt = TkEtMiss->etMiss();
}

void L1Analysis::L1AnalysisTrackMET::SetGenMET(const edm::Handle<reco::GenMET> genMET, unsigned maxL1Extra)
{
  l1extra_.gensumEt = genMET->sumEt();
}